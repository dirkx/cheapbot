#include <Serial.h>

#include "leds.h"
#include "motor.h"
#include "speaker.h"

// All these - LOW when pressed, HIGH when not.
#define BUT1 (16)      // Button 1, P1.6; pull-up; moment button to ground, Front right
#define BUT2 (17)      // Button 2, P1.7; pull-up; moment button to ground, Front left
#define BUT_RST (57)   // Reset button; P5.7/RST; on aft
#define BUT_BOOT (06)  // Boot button when jumpered on bottom PCB; P0.6/AD6/URI; on aft

void setup() {
  led_init();
  speaker_init();
  motor_init();

  USBSerial_println("Button test started");

  // buttons are wired to ground - so pull up desired (though PCB has resistors in v1.00)
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  // These are wired to the 3V3 (as their main function requires this) */
  pinMode(BUT_BOOT, INPUT);
  pinMode(BUT_RST, INPUT);
}

void loop() {

  // Show the status of the two front buttons every
  // 5 seconds via USB
  //
  {
    static unsigned int _lst = 0;
    if (millis() - _lst > 5 * 1000) {
      _lst = millis();
      USBSerial_print("BUT1 (left front):  ");
      USBSerial_println(digitalRead(BUT1) ? "off" : "Pressed");
      USBSerial_print("BUT2 (right front): ");
      USBSerial_println(digitalRead(BUT2) ? "off" : "Pressed");
    }
  }

  // Light up a nearby LED when the button is pressed.
  //
  // buttons are pull up and wired to ground - hence the '!' negation.
  digitalWrite(LR1, !digitalRead(BUT1));
  digitalWrite(LL1, !digitalRead(BUT2));
  
  // On v1.00 - wired wrongly to ground; on v1.01 as RST below
  digitalWrite(LL3, !digitalRead(BUT_BOOT));

  // button are wired to the 3V3 - so no negation needed.
  digitalWrite(LR3, digitalRead(BUT_RST));

  // Buzz the speaker while a front button is pressed.
  //
  speaker((!digitalRead(BUT1) || !digitalRead(BUT2)));
}
