#include <Arduino.h>

class QMic {
   public:
    QMic(byte micPin, uint16_t sampleWindow) : _micPin(micPin), _sampleWindow(sampleWindow) {};

    void loop(unsigned long currentMillis) {
        if (_startMillis == NULL) _startMillis = currentMillis;

        if (currentMillis - _startMillis < _sampleWindow) {
            _sample = analogRead(_micPin);

            if (_sample < 1024 && _sample > _signalMax) {
                _signalMax = _sample;
            } else if (_sample < 1024 && _sample < _signalMin) {
                _signalMin = _sample;
            }
        }

        else {
            _volume = _signalMax - _signalMin;
            _startMillis = currentMillis;
            _signalMax = 0;
            _signalMin = 1024;
        }
    }

    uint16_t volume() { return _volume; }

   private:
    byte _micPin;
    uint16_t _sampleWindow;
    uint16_t _volume = 0;
    uint16_t _sample;
    uint16_t _signalMax;
    uint16_t _signalMin;
    unsigned long _startMillis;
};