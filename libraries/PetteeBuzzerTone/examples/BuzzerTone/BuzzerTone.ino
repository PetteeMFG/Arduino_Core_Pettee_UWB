#define PIN_BUZZER PA1

void setup()
{
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop()
{
  noTone(PIN_BUZZER);         // turn off tone function on buzzer pin
  tone(PIN_BUZZER, 500, 200); // play a note on buzzer pin for 200 ms
  delay(1000);
}