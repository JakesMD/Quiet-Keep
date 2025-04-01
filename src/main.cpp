#include <Arduino.h>

#include "buzzer.h"
#include "config.h"
#include "fade_led.h"
#include "mic.h"
#include "pot.h"

QMic mic(MIC_PIN, SAMPLE_WINDOW);
QFadeLED led(LED_PIN);
QPot sensitivityPot(POT_SENSITIVITY_PIN, 0, 400);
QPot durationPot(POT_DURATION_PIN, 300, 0);
QBuzzer buzzer(BUZZER_PIN, BUZZER_FREQUENCY, FAST_FADE_DURATION);

uint16_t volume;
uint16_t accumulation;
uint16_t accumulationTriggerVolume;
uint16_t accumulationLimit;
unsigned long currentMillis;

enum ACCUMULATION_LEVEL {
    LOW_LEVEL,
    MEDIUM_LEVEL,
    HIGH_LEVEL,
};

uint16_t calculateAccumulation(uint16_t volume, uint16_t currentAccumulation, uint16_t accumulationTriggerVolume,
                               uint16_t accumulationLimit) {
    uint16_t newAccumulation;

    if (volume >= accumulationTriggerVolume && currentAccumulation < accumulationLimit) {
        newAccumulation = currentAccumulation + ATTACK;
    } else if (volume < accumulationTriggerVolume && currentAccumulation > 0) {
        newAccumulation = currentAccumulation - RELEASE;
    }

    if (newAccumulation > accumulationLimit) newAccumulation = accumulationLimit;

    return newAccumulation;
}

ACCUMULATION_LEVEL calculateAccumulationLevel(uint16_t accumulation, uint16_t accumulationLimit) {
    if (accumulation > accumulationLimit / 2) {
        return HIGH_LEVEL;
    } else if (accumulation > accumulationLimit / 4) {
        return MEDIUM_LEVEL;
    } else {
        return LOW_LEVEL;
    }
}

void plotData(uint16_t volume, uint16_t accumulation, uint16_t accumulationTriggerVolume, uint16_t accumulationLimit) {
    Serial.print("Volume:");
    Serial.print(volume);
    Serial.print(",");
    Serial.print("Accumulation:");
    Serial.print(accumulation * 10);
    Serial.print(",");
    Serial.print("Accumulation_Trigger_Volume:");
    Serial.print(accumulationTriggerVolume);
    Serial.print(",");
    Serial.print("Accumulation_Limit:");
    Serial.println(accumulationLimit * 10);
    Serial.print(",");
    Serial.print("High_Level:");
    Serial.print((accumulationLimit * 10) / 2);
    Serial.print(",");
    Serial.print("Medium_Level:");
    Serial.print((accumulationLimit * 10) / 4);
}

void setup() { Serial.begin(115200); }

void loop() {
    currentMillis = millis();

    mic.loop(currentMillis);
    led.loop(currentMillis);
    buzzer.loop(currentMillis);
    sensitivityPot.loop(currentMillis);
    durationPot.loop(currentMillis);

    volume = mic.volume();
    accumulationTriggerVolume = sensitivityPot.value();
    accumulationLimit = durationPot.value();

    accumulation = calculateAccumulation(volume, accumulation, accumulationTriggerVolume, accumulationLimit);

    switch (calculateAccumulationLevel(accumulation, accumulationLimit)) {
        case HIGH_LEVEL:
            led.setFadeDuration(FAST_FADE_DURATION);
            buzzer.enable();
            break;
        case MEDIUM_LEVEL:
            led.setFadeDuration(MEDIUM_FADE_DURATION);
            buzzer.disable();
            break;
        case LOW_LEVEL:
            led.setFadeDuration(SLOW_FADE_DURATION);
            buzzer.disable();
            break;
    }

    plotData(volume, accumulation, accumulationTriggerVolume, accumulationLimit);
}
