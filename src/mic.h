#include <Arduino.h>

class QMic {
   public:
    QMic(byte micPin, uint16_t sampleWindow) : _micPin(micPin) {};

    void loop() {
        _sample = analogRead(_micPin);

        if (_sample < 1024 && _sample > _signalMax) _signalMax = _sample;

        if (_sample < 1024 && _sample < _signalMin) _signalMin = _sample;

        _volume = _signalMax - _signalMin;
    }

    uint16_t volume() {
        _signalMax = 0;
        _signalMin = 1024;
        return _volume;
    }

   private:
    byte _micPin;
    uint16_t _volume = 0;
    uint16_t _sample;
    uint16_t _signalMax = 0;
    uint16_t _signalMin = 1024;
};