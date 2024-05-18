/*
  Attach Interrupts

  Add counter value using attachInterrupt function. 

  The circuit:
  - Connect a push button between P3.2 and ground
    Connect another push button between P3.3 and ground
    No debouncing is implemented here. Add a debouncing capacitor if needed. 
  
  created 2020
  by Deqing Sun for use with CH55xduino

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
*/
#include <Serial.h>

volatile uint8_t ledState = 0, oLedState = 0;

#pragma save
#pragma nooverlay
void timer3_interrupt(void) __interrupt(INT_NO_TMR3) { /* Timer3 interrupt service routine (ISR) */
  ledState++;
}
#pragma restore

void setup_timer() {
  EA = 1;  /* Enable global interrupt */
  ET2 = 1; /* Enable timer3 interrupt */

#define DIVISOR (1000)
#define TICKS (100 * F_CPU / 1000 / DIVISOR)

  T3_SETUP = bT3_IE_END | bT3_EN_CK_SE;

  T3_COUNT_L = DIVISOR  & 0xFF;
  T3_COUNT_H = (DIVISOR >> 8) & 0xFF;
  T3_END_H = TICKS >> 8;
  T3_END_L = TICKS & 0xFF;
  T3_CTRL |= bT3_CAP_M1; /* Mode 2 - auto */
  T3_CTRL |= bT3_CNT_EN; /* Enable timer */
}

void setup() {
  setup_timer();
}

void loop() {
  if (ledState != oLedState) {
    oLedState = ledState;
    USBSerial_println("timer fired");
  }
}
