#define PIN_LED_GREEN PA7

void setup()
{
    pinMode(PIN_LED_GREEN, OUTPUT);    // Set green LED pin to digital output
    digitalWrite(PIN_LED_GREEN, HIGH); // Set green LED pin to high voltage (Turns off the LED)
}

void loop()
{
    digitalWrite(PIN_LED_GREEN, LOW); // Set green LED pin to low voltage (Turns on the LED)
    delay(1000);
    digitalWrite(PIN_LED_GREEN, HIGH); // Set green LED pin to high voltage (Turns off the LED)
    delay(1000);
}