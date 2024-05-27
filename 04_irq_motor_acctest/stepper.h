const uint32_t stepsPerRevolution = 2048;
const uint32_t distancePerRevolution = 140;  // millimeter (~d x PI)
const uint16_t maxSpeed = 600;               // mm/second
const uint16_t speedAcc = 300;               // mm/second/second -- go to topspeed in 1 second.
const uint16_t tocksPerSecond = 3000;        // 5kHz stepping speed (max, just above max speed)
const uint16_t accAdjustTock = 50;
const uint16_t speedInc = 20;  // speedAcc * accAdjustTock / tocksPerSecond;

const uint8_t STEP_PATTERN[] = { 0b0001, 0b011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001 };
const uint8_t STOP_PATTERN = 0b000;

typedef struct {
  uint32_t steps_to_take;
  uint32_t ramp;
  uint32_t steps;
  uint32_t speed;
  uint8_t portbase;
  uint16_t skip;
  uint8_t flags;
} motor;

#define INREVERSE(x) ((x)->flags & 1)

motor left = { .portbase = 23 }, right = { .portbase = 33 };

static volatile uint32_t tocks = 0;


void initMotor(motor* m, uint8_t portBase) {
  m->steps_to_take = 0;
  m->steps = 0;
  m->speed = 0;
  m->portbase = portBase;
  m->skip = 1;
  for (int p = 0; p < 4; p++) {
    digitalWrite(portBase + p, LOW);
    pinMode(portBase + p, OUTPUT);
  }
}

void stepMotor(motor* m) {
  // Early exit when there is nothing to do or
  // if it is not our time yet.
  //
  if (!m->steps_to_take)
    return;

  // stop the motor when we're done
  if (m->steps > m->steps_to_take) {
    m->steps = m->steps_to_take = 0;
    m->skip = 1;
    for (int i = 0; i < 4; i++)
      digitalWrite(m->portbase + i, (STOP_PATTERN & (1 << i)) ? HIGH : LOW);
    return;
  };

  if (0 == tocks % m->skip) {
    // take a step
    uint8_t at = m->steps & 7;
    if (INREVERSE(m))
      at = 7 - at;
    // m->portbase &= 0x0F + STEP_PATTERN[at]<<3;
    for (int i = 0; i < 4; i++)
      digitalWrite(m->portbase + i, (STEP_PATTERN[at] & (1 << i)) ? HIGH : LOW);

    m->steps++;
  };

  // See if it is time to adjust acceleration
  if (0 == tocks % accAdjustTock) {
    if (m->steps < m->steps_to_take / 2) {
      // We are before the half way point.

      // Already at max speed ?
      if (m->speed >= maxSpeed) {
        digitalWrite(42, LOW);
        digitalWrite(44, HIGH);
        return;
      };
      digitalWrite(42, HIGH);

      // We are not yet at the halfway point. Allow to go faster
      //
      m->speed += speedInc;
      if (m->speed >= maxSpeed) {
        m->speed = maxSpeed;
        m->ramp = m->steps;
      }
    } else {
      // We're past the halfway point - slow down at the tail end
      //
      // if (m->speed < speedInc * (m->steps_to_take - m->steps))
      if (m->steps < (m->steps_to_take - m->ramp)) {
        digitalWrite(43, LOW);
        return;
      };
      digitalWrite(43, HIGH);
      digitalWrite(44, LOW);

      m->speed -= speedInc;
      if (m->speed < 1)
        m->speed = 1;
    }
    // Update the time/skip
    m->skip = 500 / m->speed;
    if (m->skip < 1) m->skip = 1;
  }
}

void handle_tock() {
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
  m->speed = 10;
  m->steps_to_take = distInMm * stepsPerRevolution / distancePerRevolution;

  USBSerial_print("MOVE update: ");
  USBSerial_println(m->steps_to_take);
  USBSerial_print("       skip: ");
  USBSerial_println(1024 / m->speed);

  USBSerial_print("Time ");
  USBSerial_println_u(tocks);
  USBSerial_print("F-CPU ");
  USBSerial_println_u(F_CPU);

}

#define isMoving() (left.steps_to_take || right.steps_to_take)

void initSteppers() {
  initMotor(&left, 23);
  initMotor(&right, 33);
}
