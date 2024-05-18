
#define LL0 (40 + 0) // Led left 0, P4.0 - white (on front)
#define LL1 (40 + 1) // Led left 1, P4.1 - yellow/orange
#define LL2 (40 + 2) // Led left 2, P4.2 - red (port side)
#define LL3 (40 + 3) // Led left 3, P4.3 - red (on back)

#define LR0 (40 + 4) // Led right 0, P4.4 - white (on front)
#define LR1 (40 + 5) // Led right 1, P4.5 - yellow/orange
#define LR2 (40 + 6) // Led right 2, P4.6 - green (star board)
#define LR3 (40 + 7) // Led right 3, P4.7 - red (on back)

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

