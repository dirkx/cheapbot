#include <Serial.h>
#include "stepper.h"
#define LED (40)
#define SPKR (12)  // Speaker/buzzer - P1.2; high == buzz, low = silent

void Timer3Interrupt(void) __interrupt(INT_NO_TMR3) __using(1) {
  // clear count-passed endpoint interupt.
  T3_STAT |= bT3_IF_END;

  handle_tock();
}

void setup() {
  for (int i = 0; i < 48; i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  };
  for (int i = 10; i < 48; i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  };
  for (int i = 20; i < 48; i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  };
  for (int i = 40; i < 48; i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  };
  for (int i = 30; i < 38; i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  };

  // give the Arduino IDE some time to switch.
  delay(3000);
  USBSerial_println(__FILE__);
  USBSerial_println("Gotta get going...");
  USBSerial_println(__DATE__);
  USBSerial_println(__TIME__);

#define PERIOD (1000)              /* micro seconds */
#define DIV (F_CPU / 1000 / 1000) /* 1 Mhz left over */
#define ENDCOUNT (PERIOD)

  // set divisor
  T3_SETUP |= bT3_EN_CK_SE;  //  Enable to accessing divisor setting register.
  T3_CK_SE_L = DIV & 0xff;
  T3_CK_SE_H = (DIV >> 8) & 0xff;
  T3_SETUP &= ~bT3_EN_CK_SE;  // clear access again.

  T3_CTRL |= bT3_CLR_ALL;   // Force clear FIFO and count of timer3.
  T3_CTRL &= ~bT3_CLR_ALL;  // And go back to reset by software
  T3_SETUP |= bT3_IE_END;   // Enable interruts
  T3_CTRL &= ~bT3_MOD_CAP;  // Timer or PWM mode

  // T3_STAT |= bT3_IF_DMA_END | bT3_IF_FIFO_OV | bT3_IF_FIFO_REQ | bT3_IF_ACT | bT3_IF_END;
  T3_STAT |= bT3_IF_END;  // Interrupt when we pass ENDCOUNT

  T3_END_L = ENDCOUNT & 0xff;
  T3_END_H = (ENDCOUNT >> 8) & 0xff;

  // *(void (*)(void)) INT_ADDR_TMR3 = &Timer3Interrupt();

  T3_CTRL |= bT3_CNT_EN;  // start timer

  IE_TMR3 = 1;  // Timer3 interrupt is enabled.
  EA = 1;       // Global interrupt enable control bit

  initSteppers();
}

void loop() {
  {
    static uint32_t last, c;
    if (millis() - last > 1000) {
      last = millis();
      USBSerial_print("Time ");
      USBSerial_print_u(tocks-c); c = tocks;
      USBSerial_print(". F-CPU ");
      USBSerial_println_u(F_CPU);
    };
  }
  if (1) {
    static uint32_t last;
    if (millis() - last > 100 || !isMoving()) {
      last = millis();
      USBSerial_print("@=");
      USBSerial_print(left.steps);
      USBSerial_print("\t Speed=");
      USBSerial_print(left.speed);
      USBSerial_print("\tSkips=");
      USBSerial_print(left.skip);
      USBSerial_print("\tRamp=");
      USBSerial_print(left.ramp);
#if 0
      USBSerial_print("****** Steps: ");
      USBSerial_print(left.steps);
      USBSerial_print(" ");
      USBSerial_print(right.steps);
      USBSerial_println("");
      USBSerial_print("     Speed: ");
      USBSerial_print(left.speed);
      USBSerial_print(" ");
      USBSerial_print(right.speed);
      USBSerial_println("");
      USBSerial_print("     Steps: ");
      USBSerial_print(left.steps_to_take);
      USBSerial_print(" ");
      USBSerial_print(right.steps_to_take);
      USBSerial_println("");
      USBSerial_print("     Skips: ");
      USBSerial_print(left.skip);
      USBSerial_print(" ");
      USBSerial_print(right.skip);
      USBSerial_println("");
      USBSerial_print("     Ramp: ");
      USBSerial_print(left.ramp);
      USBSerial_print(" ");
      USBSerial_print(right.ramp);
      USBSerial_println("");
#endif
      USBSerial_println("");
    }
  }
  if (!isMoving()) {
    digitalWrite(SPKR, HIGH);
    delay(2);
    digitalWrite(SPKR, LOW);

    digitalWrite(41, LOW);
    USBSerial_println("Done - pause");
    delay(1000);

    digitalWrite(SPKR, HIGH);
    delay(5);
    digitalWrite(SPKR, LOW);

    digitalWrite(41, HIGH);
    USBSerial_println("FWD 500");

    moveMm(&left, 500);
  };
}
