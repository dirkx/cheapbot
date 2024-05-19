#include <Serial.h>
#define LED (40)

static unsigned int flag = 0;
void Timer3Interrupt(void) __interrupt(INT_NO_TMR3) __using(1) {
  // T3_CTRL &= ~bT3_CNT_EN;  // stop timer

  flag++;
  digitalWrite(LED, flag & 1);

  // clear count-passed endpoint interupt.
  T3_STAT |= bT3_IF_END;
  
  // T3_CTRL |= bT3_CNT_EN;  // re-start timer
}

void setup() {
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

#define PERIOD (500)            /* milli seconds */
#define DIV (F_CPU / 10 / 1000) /* 10 Khz left over */
#define ENDCOUNT (10 * PERIOD)  /* count to 10k */

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
}

void loop() {
  {
    static uint32_t last;
    if (millis() - last > 2500) {
      last = millis();
      USBSerial_println("Tock");
    }
  }
  {
    static uint32_t last = 0;
    if (last != flag) {
      last = flag;
      USBSerial_print("IRQ seen:");
      USBSerial_println(flag);
      delay(100);
    }
  }
}
