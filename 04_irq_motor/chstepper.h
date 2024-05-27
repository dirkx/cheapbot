const uint32_t stepsPerRevolution = 2038;
const uint32_t distancePerRevolution = 140;  // millimeter (~d x PI)
const uint16_t maxSpeed = 300;               // mm/second
const uint16_t speedAcc = 300;               // mm/second/second -- go to topspeed in 1 second.
const uint16_t tocksPerSecond = 3000;        // 5kHz stepping speed (max, just above max speed)
const uint16_t accAdjustTock = 50;
const uint16_t speedInc = speedAcc * accAdjustPerSecond / tocksPerSecond;

// https://www.instructables.com/BYJ48-Stepper-Motor/
////   step 0 1 2 3 4 5 6 7 
// red.     - - - - - - - -  common
// orange   x x           x 
// yellow     x x x
// pink           x x x 
// blue               x x x
const uint8_t STEP_PATTERN[] = { 0b0001, 0b011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001 };
const uint8_t STOP_PATTERN = 0xb000;

typedef struct {
  uint32_t steps_to_take;
  uint16_t steps;
  uint8_t speed;
  uint8_t portbase;
  uint16_t skip;
  uint8_t portbase;
  uint8_t flags;
} motor;

#define INREVERSE(x) ((x)->flags & 1)

motor left = { .portbase = 23 }, right = { .portbase 33 };

static volatile uint32_t tocks = 0;

void initMotor(motor* m, uint8_t portBase) {
  m->steps_to_take = 0;
  m->steps = 0;
  m->speed = 0;
  m->portbase = portBase;
  m->skip = 1;
}

void stepMotor(motor* m) {
  // Early exit when there is nothing to do or
  // if it is not our time yet.
  //
  if (!m->steps_to_take)
    return;

  // stop the motor when we're done
  if (m->steps_to_take == 0) {
    m->steps_to_take = 0;
    m->skip = 1;
    m->portbase &= 0xF0 + STOP_PATTERN;
    return;
  };

  if (0 == tocks % m->skip) {
    // take a step
    uint8_t at = m->steps & 7;
    if (INREVERSE(m))
      at = 7 - at;
    m->portbase &= 0xF0 + STEP_PATTERN[at];
    m->steps++;
  };

  // See if it is time to adjust acceleration
  if (0 == tocks % accAdjustPerSecond) {

    // See if we need to change speed.
    if (m->steps < m->steps_to_take / 2) {
      if (m->speed > maxSpeed)
        return;

      // We are not yet at the halfway point. Allow to go faster
      //
      m->speed += speedInc;
      if (m->speed > maxSpeed)
        m->speed = maxSpeed;
    } else {
      // We're past the halfway point - slow down at the tail end
      //
      if (m->speed > speedInc * (m->steps_to_take - m->steps)) )
      return;

      m->speed -= speedInc;
      if (m->speed < 1)
        m->speed = 1;
    }
    // Update the time/skip
    m->skip = 1024 / m->speed;
  }
}

void irq() {
  tocks++;
  stepMotor(&left);
  stepMotor(&right);
}

void moveMm(motor* m, int distInMm) {
  if (distInMm < 0) {
    distInMm = -distInMm;
    m->flags |= 1;
  } else {
    m->flags &= ~1;
  };
  m->steps_to_take = distInMm / distancePerRevolution * stepsPerRevolution;
}

#define isMoving() (left.steps_to_take || right.steps_to_take)
