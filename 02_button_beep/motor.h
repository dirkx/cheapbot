// Motor - red is middel for H/V coils
//
#define MOTORLEFT0 (20) // blue
#define MOTORLEFT1 (21) // pink
#define MOTORLEFT2 (22) // yellow 
#define MOTORLEFT3 (23) // orange
#define MOTORLEFT4 (24) // TP 1 (on back of board)
#define MOTORLEFT5 (25) // TP 2
#define MOTORLEFT6 (26) // TP 3

// Motor - red is middel for H/V coils
//
#define MOTORRIGHT0 (30) // blue - coil 4 (H, pos)
#define MOTORRIGHT1 (31) // pink - coil 2 (V, pos)
#define MOTORRIGHT2 (32) // yellow - coil 3 (H, rev)
#define MOTORRIGHT3 (33) // orange - coil 1 (V, rev)
#define MOTORRIGHT4 (34) // TP 4
#define MOTORRIGHT5 (35) // TP 5
#define MOTORRIGHT6 (36) // TP 6

const unsigned int stepsPerRevolution = 2038;

// pinout both motors
//
const uint8_t MOTOR_LEFT = 0;
const uint8_t MOTOR_RIGHT = 1;

const uint8_t MOTOR_GPIO[2][4] = {
  //  {MOTORLEFT3, MOTORLEFT1, MOTORLEFT2, MOTORLEFT0},
  //  {MOTORRIGHT3, MOTORRIGHT1, MOTORRIGHT2, MOTORRIGHT0}
  //  {MOTORLEFT0, MOTORLEFT1, MOTORLEFT2, MOTORLEFT3},
  //  {MOTORRIGHT1, MOTORRIGHT3, MOTORRIGHT0, MOTORRIGHT2}
  //  {MOTORLEFT3, MOTORLEFT2, MOTORLEFT1, MOTORLEFT0},
  //  {MOTORRIGHT0, MOTORRIGHT1, MOTORRIGHT2, MOTORRIGHT3}
  {MOTORLEFT0, MOTORLEFT3, MOTORLEFT1, MOTORLEFT2},
  {MOTORRIGHT2, MOTORRIGHT3, MOTORRIGHT1, MOTORRIGHT0}
};

const uint8_t STEP_PATTERN[] = {0b0001, 0b011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};
uint8_t motor_state[2] = { 0, 0 };

void motor_gpio(uint8_t *p, uint8_t v)
{
  for (int i = 0; i < 4; ++i)
    digitalWrite(p[i], (v & (1 << i)) ? HIGH : LOW );
}

void motor_set(uint8_t motor, uint8_t v)
{
  if (motor_state[motor] == v) {
    motor_gpio(MOTOR_GPIO[motor], 0);
    return;
  };

  motor_gpio(MOTOR_GPIO[motor], STEP_PATTERN[v & 0x7]);
  motor_state[motor] = v;
}

void motor_step(uint8_t motor, int dir) {
  motor_set(motor, (motor_state[motor] + dir + 4) & 7);
}

void motor_init() {
  for (uint8_t i = 0; i < 2; ++i) {
    for (uint8_t j = 0; j < 4; ++j)
      pinMode(MOTOR_GPIO[i][j], OUTPUT);
    motor_gpio(MOTOR_GPIO[i], 0);
  };

}
