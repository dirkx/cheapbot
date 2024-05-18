/* CH558 selected
    Bootloader P5.1 (USB D+) - pullup

*/
#define LR0 (40 + 4) // Led right 0, P4.4 - white (on front)
#define LR1 (40 + 5) // Led right 1, P4.5 - yellow/orange
#define LR2 (40 + 6) // Led right 2, P4.6 - green (star board)
#define LR3 (40 + 7) // Led right 3, P4.7 - red (on back)

#define LL0 (40 + 0) // Led left 0, P4.0 - white (on front)
#define LL1 (40 + 1) // Led left 1, P4.1 - yellow/orange
#define LL2 (40 + 2) // Led left 2, P4.2 - red (port side)
#define LL3 (40 + 3) // Led left 3, P4.3 - red (on back)

uint8_t right[4] = { LR0, LR1, LR2, LR3 };
uint8_t left[4] = { LL0, LL1, LL2, LL3 };

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(left[i], OUTPUT);
    pinMode(right[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
int v = 0;
int i = 0;
void loop() {


  digitalWrite(left[i], LOW);
  digitalWrite(right[i], LOW);

  i++;
  i = i % 4;

  digitalWrite(left[i], HIGH);
  digitalWrite(right[i], HIGH);

  delay(200);
}
