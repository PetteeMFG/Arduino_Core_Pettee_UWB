#include "MP2695.h"
#include <InputDebounce.h>

#define PIN_BUZZER PA1
#define PIN_LED_RED PA6
#define PIN_LED_GREEN PA7
#define PIN_BATTERY_VOLTAGE PC2  // Ranges from 3.0V (465) to 4.23V (658)
#define PIN_BQ51003_CHG PA15     // Open-drain output – active when OUT is enabled
#define PIN_BQ51003_TS_CTRL PB10 // Pull below V CTRL-Low (0.05V) or above V CTRL-High (0.2V) to send an "End Power Transfer Packet"
#define PIN_BQ51003_EN PC0       // Push-pull driver for external PFET when wired charging is active
#define PIN_MP2695_IRQ PB5
#define INPUT_DEBOUNCE_DELAY 0

MP2695 charger;
static InputDebounce inputBQ51003CHG;
static bool isCharging = false;
static bool chargingLedOn = false;
static bool batteryListFull = false;
static unsigned long loopDelay = 0;
static float batteryVoltageList[10];
static float batteryPercentList[10];
static int batteryPos = 0;

void setup()
{
    // Initialize Serial communication for debugging
    Serial.begin(115200);
    Serial.println(F("Pettee Battery Management Test Sketch"));

    // Initialize MP2695 charger with 1200mAh 3.7V battery settings
    charger.init();

    // Initialize buzzer, LEDs, and battery voltage pins
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_RED, HIGH);
    pinMode(PIN_LED_GREEN, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW);

    // Initialize highly integrated wireless receiver Qi pins
    inputBQ51003CHG.registerCallbacks(inputBQ51003CHG_pressedCallback, inputBQ51003CHG_releasedCallback, NULL, NULL);
    inputBQ51003CHG.setup(PIN_BQ51003_CHG, INPUT_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
}

void loop()
{
    // Process input callback
    unsigned long now = millis();
    inputBQ51003CHG.process(now);

    if (now >= loopDelay)
    {
        loopDelay = now + 500;

        if (isCharging)
        {
            if (!chargingLedOn)
            {
                chargingLedOn = true;
                digitalWrite(PIN_LED_RED, LOW);
            }
            else
            {
                chargingLedOn = false;
                digitalWrite(PIN_LED_RED, HIGH);
            }
        }

        float batteryVoltageInput = analogRead(PIN_BATTERY_VOLTAGE);
        float batteryVoltage = mapf(batteryVoltageInput, 465.0F, 658.0F, 3.0F, 4.23F);
        float batteryPercent = mapf(batteryVoltageInput, 465.0F, 658.0F, 0.0F, 100.0F);

        batteryVoltageList[batteryPos] = batteryVoltage;
        batteryPercentList[batteryPos] = batteryPercent;
        batteryPos++;

        if (batteryPos >= 10)
        {
            batteryPos = 0;
            batteryListFull = true;
        }

        float batteryVoltageAve = 0.0F;
        float batteryPercentAve = 0.0F;
        int numBatteryList = 10;
        if (!batteryListFull)
            numBatteryList = batteryPos;
        for (int i = 0; i < numBatteryList; i++)
        {
            batteryVoltageAve += batteryVoltageList[i];
            batteryPercentAve += batteryPercentList[i];
        }
        batteryVoltageAve /= numBatteryList;
        batteryPercentAve /= numBatteryList;

        if (batteryVoltageAve >= 4.15)
        {
            isCharging = false;
            chargingLedOn = false;
            digitalWrite(PIN_LED_RED, HIGH);
        }

        Serial.print(F("Battery Level: "));
        Serial.print(batteryVoltageAve);
        Serial.print(F("V | "));
        Serial.print(batteryPercentAve);
        Serial.println(F("%"));
    }
}

// Float mapping utility function (Used in battery and duty cycle calculations)
float mapf(float input, float input_min, float input_max, float output_min, float output_max)
{
    if (input < input_min)
        return output_min;
    else if (input > input_max)
        return output_max;
    else
        return (input - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
}

void inputBQ51003CHG_pressedCallback(uint8_t pinIn)
{
    isCharging = true;
    tone(PIN_BUZZER, 250, 250);
}

void inputBQ51003CHG_releasedCallback(uint8_t pinIn)
{
    isCharging = false;
    chargingLedOn = false;
    digitalWrite(PIN_LED_RED, HIGH);
}