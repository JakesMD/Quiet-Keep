
#include <Arduino.h>

class QBuzzer {
   public:
    QBuzzer(byte pin, uint16_t frequency, uint16_t beepDuration)
        : _pin(pin), _frequency(frequency), _beepDuration(beepDuration) {}

    void loop(unsigned long currentMillis) {
        if (!_isEnabled) return;

        if (currentMillis - _startMillis >= _beepDuration || _startMillis == NULL) {
            _startMillis = currentMillis;

            if (_isBeepOn) {
                _isBeepOn = false;
                noTone(_pin);
            } else {
                _isBeepOn = true;
                tone(_pin, _frequency);
            }
        }
    }

    void enable() { _isEnabled = true; }

    void disable() {
        _isEnabled = false;
        _isBeepOn = false;
        _startMillis = NULL;
        noTone(_pin);
    }

   private:
    byte _pin;
    uint16_t _frequency;
    uint16_t _beepDuration;
    unsigned long _startMillis;
    bool _isEnabled = false;
    bool _isBeepOn = false;
};