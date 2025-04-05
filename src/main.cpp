#include <Arduino.h>

#include "buzzer.h"
#include "config.h"
#include "fade_led.h"
#include "mic.h"
#include "pot.h"

QMic mic(MIC_PIN, SAMPLE_WINDOW);
QFadeLED led(LED_PIN);
QPot sensitivityPot(POT_SENSITIVITY_PIN, 400, 0);
QBuzzer buzzer(BUZZER_PIN, BUZZER_FREQUENCY, FAST_FADE_DURATION);

uint16_t volume = 0;
uint16_t accumulation = 0;
uint16_t accumulationTriggerVolume;
unsigned long currentMillis;
unsigned long startMillis;

uint16_t calculateAccumulation(uint16_t volume, uint16_t currentAccumulation, uint16_t accumulationTriggerVolume) {
    uint16_t newAccumulation = 0;

    if (volume >= accumulationTriggerVolume && currentAccumulation < ACCUMULATION_LIMIT) {
        newAccumulation = currentAccumulation + ATTACK;
    } else if (volume < accumulationTriggerVolume && currentAccumulation > 0) {
        newAccumulation = currentAccumulation - RELEASE;
    }

    if (newAccumulation > ACCUMULATION_LIMIT) newAccumulation = ACCUMULATION_LIMIT;

    return newAccumulation;
}

void plotData(uint16_t volume, uint16_t accumulation, uint16_t accumulationTriggerVolume) {
    Serial.print("Volume:");
    Serial.print(volume);
    Serial.print("\t");
    Serial.print("Accumulation:");
    Serial.print(accumulation);
    Serial.print("\t");
    Serial.print("Accumulation_Trigger_Volume:");
    Serial.print(accumulationTriggerVolume);
    Serial.print("\t");
    Serial.print("Accumulation_Limit:");
    Serial.println(ACCUMULATION_LIMIT);
}

void setup() {
    Serial.begin(115200);
    currentMillis = millis();
}

void loop() {
    startMillis = currentMillis;

    while (currentMillis - startMillis < SAMPLE_WINDOW) {
        led.loop(currentMillis);
        mic.loop();
        currentMillis = millis();
    }

    volume = mic.volume();
    accumulationTriggerVolume = sensitivityPot.read();
    accumulation = calculateAccumulation(volume, accumulation, accumulationTriggerVolume);

    plotData(volume, accumulation, accumulationTriggerVolume);

    if (accumulation >= ACCUMULATION_LIMIT) {
        led.setFadeDuration(FAST_FADE_DURATION);

        while (buzzer.beepCount() < 3) {
            currentMillis = millis();
            led.loop(currentMillis);
            buzzer.loop(currentMillis);
        }

        buzzer.reset();
        led.setFadeDuration(MEDIUM_FADE_DURATION);
        accumulation = ACCUMULATION_RESET_VALUE;
    } else if (accumulation > 0) {
        led.setFadeDuration(MEDIUM_FADE_DURATION);
    } else {
        led.setFadeDuration(SLOW_FADE_DURATION);
    }
}