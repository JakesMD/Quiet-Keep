
#include <Arduino.h>

class QBuzzer {
   public:
    QBuzzer(byte pin, uint16_t frequency, uint16_t beepDuration)
        : _pin(pin), _frequency(frequency), _beepDuration(beepDuration) {}

    void loop(unsigned long currentMillis) {
        if (currentMillis - _startMillis >= _beepDuration || _startMillis == 0) {
            _startMillis = currentMillis;

            if (_isBeepOn) {
                _isBeepOn = false;
                _beepCount++;
                noTone(_pin);
            } else {
                _isBeepOn = true;
                tone(_pin, _frequency);
            }
        }
    }

    void reset() {
        _isBeepOn = false;
        _startMillis = 0;
        _beepCount = 0;
        noTone(_pin);
    }

    byte beepCount() { return _beepCount; }

   private:
    byte _pin;
    uint16_t _frequency;
    uint16_t _beepDuration;
    unsigned long _startMillis = 0;
    bool _isBeepOn = false;
    byte _beepCount = 0;
};