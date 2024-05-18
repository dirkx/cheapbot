
#define SPKR (12)  // Speaker/buzzer - P1.2; high == buzz, low = silent

void speaker(bool onOff) {
  digitalWrite(SPKR, onOff ? HIGH : LOW);
}

void speaker_init() {
  pinMode(SPKR, OUTPUT);
  speaker(false);
}