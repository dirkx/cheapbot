// #include <Serial.h>

void Timer3Interrupt(void) __interrupt {
  T3_CTRL &= ~bT3_CNT_EN;

  digitalWrite(40, LOW);

  // clear count-passed endpoint interupt.
  if (T3_STAT & bT3_IF_END)
    T3_STAT |= bT3_IF_END;

  T3_CTRL |= bT3_CNT_EN;  // re-start timer
}

void setup() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(40 + i, LOW);
    pinMode(40 + i, OUTPUT);
  }
  digitalWrite(40, HIGH);

#if 0
  // give the Arduino IDE some time to switch.
  delay(3000);
  USBSerial_println(__FILE__);
  USBSerial_println("Gotta get going...");
  USBSerial_println(__DATE__);
  USBSerial_println(__TIME__);
#endif


#define DIV (0x4000)      // Divisor of Fsys
#define ENDCOUNT (0x4000)  // Count

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

  T3_CTRL |= bT3_CNT_EN;  // start timer

  IE_TMR3 = 1;  // Timer3 interrupt is enabled.
  EA = 1;       // Global interrupt enable control bit
}

void loop() {
#if 0
  {
    static uint32_t last;
    if (millis() - last > 2500) {
      last = millis();
      USBSerial_println("Tock");
    }
  }
  {
    static uint32_t last = 0;
    if (last != millis_int) {
      last = millis_int;
      USBSerial_print("IRQ seen:");
      USBSerial_println(millis_int);
      delay(500);
    }
  }
  #endif
}
