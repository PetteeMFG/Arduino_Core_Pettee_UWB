#define PIN_LED_RED PA6

void setup()
{
    pinMode(PIN_LED_RED, OUTPUT);    // Set red LED pin to digital output
    digitalWrite(PIN_LED_RED, HIGH); // Set red LED pin to high voltage (Turns off the LED)
}

void loop()
{
    digitalWrite(PIN_LED_RED, LOW); // Set red LED pin to low voltage (Turns on the LED)
    delay(1000);
    digitalWrite(PIN_LED_RED, HIGH); // Set red LED pin to high voltage (Turns off the LED)
    delay(1000);
}