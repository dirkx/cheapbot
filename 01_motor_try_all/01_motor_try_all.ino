/* CH559 selected
     Bootloader P5.1 (USB D+) - pullup
*/
#include <Serial.h>
#define SPKR (12)  // Speaker/buzzer - P1.2; high == buzz, low = silent

#define LL0 (40 + 0)  // Led left 0, P4.0 - white (on front)
#define LL1 (40 + 1)  // Led left 1, P4.1 - yellow/orange
#define LL2 (40 + 2)  // Led left 2, P4.2 - red (port side)
#define LL3 (40 + 3)  // Led left 3, P4.3 - red (on back)

#define LR0 (40 + 4)  // Led right 0, P4.4 - white (on front)
#define LR1 (40 + 5)  // Led right 1, P4.5 - yellow/orange
#define LR2 (40 + 6)  // Led right 2, P4.6 - green (star board)
#define LR3 (40 + 7)  // Led right 3, P4.7 - red (on back)

uint8_t led_right[4] = { LR0, LR1, LR2, LR3 };
uint8_t led_left[4] = { LL0, LL1, LL2, LL3 };

void led_init() {
  for (int i = 0; i < 4; i++) {
    pinMode(led_left[i], OUTPUT);
    digitalWrite(led_left[i], LOW);

    pinMode(led_right[i], OUTPUT);
    digitalWrite(led_right[i], LOW);
  }
}
const uint32_t stepsPerRevolution = 4076/2;

// Motor - red is middel for H/V coils
//
#define MOTORLEFT0 (20)  // TP 1 (back of board)
#define MOTORLEFT1 (21)  // TP 2
#define MOTORLEFT2 (22)  // TP 3
#define MOTORLEFT3 (23)  // orange - coil 1 (V, rev)
#define MOTORLEFT4 (24)  // yellow - coil 3 (H, rev)
#define MOTORLEFT5 (25)  // pink - coil 2 (V, pos)
#define MOTORLEFT6 (26)  // blue - coil 4 (H, pos)

// Motor - red is middel for H/V coils
//
#define MOTORRIGHT0 (30)  // TP 6
#define MOTORRIGHT1 (31)  // TP 5
#define MOTORRIGHT2 (32)  // TP4
#define MOTORRIGHT3 (33)  // orange - coil 1 (V, rev)
#define MOTORRIGHT4 (34)  // yellow - coil 3 (H, rev)
#define MOTORRIGHT5 (35)  // pink - coil 2 (V, pos)
#define MOTORRIGHT6 (36)  //

// pinout both motors
//
const uint8_t MOTOR_LEFT = 0;
const uint8_t MOTOR_RIGHT = 1;

const uint8_t MOTOR_GPIO[2][4] = {
  { MOTORLEFT3, MOTORLEFT4, MOTORLEFT5, MOTORLEFT6 },
  { MOTORRIGHT3, MOTORRIGHT4, MOTORRIGHT5, MOTORRIGHT6 }
};

const uint8_t orders[][4] = {
#if 0
  { 1, 2, 3, 4 },  // 00 a
  { 1, 2, 4, 3 },
  { 1, 3, 2, 4 },
  { 1, 3, 4, 2 },
  { 1, 4, 2, 3 },
  { 1, 4, 3, 2 },
  { 2, 1, 4, 3 },  // 06
  { 2, 1, 3, 4 },
  { 2, 3, 1, 4 },
  { 2, 3, 4, 1 },
  { 2, 4, 1, 3 },
  { 2, 4, 3, 1 },

  { 3, 1, 2, 4 },  // 12
  { 3, 1, 4, 2 },
#endif
  { 3, 2, 1, 4 },
#if 0
  { 3, 2, 4, 1 },
#endif
  { 3, 4, 1, 2 },
#if 0
  { 3, 4, 2, 1 },

#endif
  { 4, 1, 2, 3 },  // 18
#if 0
  { 4, 1, 3, 2 },
  { 4, 2, 1, 3 },
  { 4, 2, 3, 1 },
  { 4, 3, 1, 2 },
#endif
  { 4, 3, 2, 1 },
};
uint8_t order = 0;

const uint8_t STEP_PATTERN[] = { 0b0001, 0b011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001 };
uint8_t motor_state[2] = { 0, 0 };


void motor_gpio(uint8_t *p, uint8_t v) {
  for (int i = 0; i < 4; ++i)
    digitalWrite(p[orders[order][i] - 1], (v & (1 << i)) ? HIGH : LOW);
}

void motor_set(uint8_t motor, uint8_t v) {
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
  for (int i = 0; i < 50; i++)
    digitalWrite(i, LOW);

  digitalWrite(led_right[0], HIGH);

#if 0
  pinMode(MOTORRIGHT0, OUTPUT);
  pinMode(MOTORRIGHT1, OUTPUT);
  pinMode(MOTORRIGHT2, OUTPUT);
  pinMode(MOTORRIGHT3, OUTPUT);

  digitalWrite(MOTORRIGHT0, LOW);
  digitalWrite(MOTORRIGHT1, LOW);
  digitalWrite(MOTORRIGHT2, LOW);
  digitalWrite(MOTORRIGHT3, LOW);

  digitalWrite(MOTORRIGHT0, HIGH);
  delay(300);
  digitalWrite(MOTORRIGHT0, LOW);
  digitalWrite(MOTORRIGHT1, HIGH);
  delay(300);
  digitalWrite(MOTORRIGHT1, LOW);
  digitalWrite(MOTORRIGHT2, HIGH);
  delay(300);
  digitalWrite(MOTORRIGHT2, LOW);
  digitalWrite(MOTORRIGHT3, HIGH);
  delay(300);
  digitalWrite(MOTORRIGHT3, LOW);

  digitalWrite(led_right[0], LOW);
#endif

  for (uint8_t i = 0; i < 2; ++i) {
    for (uint8_t j = 0; j < 4; ++j)
      pinMode(MOTOR_GPIO[i][j], OUTPUT);
    motor_gpio(MOTOR_GPIO[i], 0);
  };

#if 1
  // Wiggle all motors.
  for (uint8_t j = 0; j < 4; ++j) {
    digitalWrite(led_left[j], HIGH);
    digitalWrite(MOTOR_GPIO[0][j], HIGH);
    digitalWrite(MOTOR_GPIO[1][j], HIGH);
    delay(300);
    digitalWrite(led_left[j], LOW);
    digitalWrite(MOTOR_GPIO[0][j], LOW);
    digitalWrite(MOTOR_GPIO[1][j], LOW);
    delay(300);
  };
#endif
}

void setup() {
  pinMode(SPKR, OUTPUT);
  digitalWrite(SPKR, LOW);

  led_init();
  motor_init();
  for (int i = 0; i < 4; i++)
    digitalWrite(led_right[i], LOW);
  for (int i = 0; i < 4; i++)
    digitalWrite(led_left[i], LOW);

  delay(1000);
  USBSerial_println("Motor test started");
  USBSerial_print("\nPermutations to try: ");
  USBSerial_println_i(sizeof(orders) / sizeof(*orders));
}

// the loop function runs over and over again forever
int steps = 0;
void loop() {
  digitalWrite(led_left[0], !digitalRead(led_left[0]));

  motor_step(MOTOR_LEFT, 1);
  // motor_step(MOTOR_RIGHT, 1);
  if (steps % stepsPerRevolution == 0) {
    static unsigned long lst = 0;
    ;
    unsigned long d = millis() - lst;
    if (lst) {
      USBSerial_print("; deltaT ");
      USBSerial_print_i(d);
      USBSerial_print(" [mSec]");
    };
    lst = millis();
    USBSerial_println("");
    digitalWrite(SPKR, HIGH);
    order = (order + 1) % (sizeof(orders) / sizeof(*orders));
    USBSerial_print("Current order: ");
    USBSerial_print_i((long)order);
    USBSerial_print(" - pinning: ");
    for (int i = 0; i < 4; i++)
      USBSerial_print_i((long)orders[order][i]);
    digitalWrite(SPKR, LOW);
  }
  steps++;
  delay(3);
}
