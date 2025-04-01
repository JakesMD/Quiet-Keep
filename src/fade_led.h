#include <Arduino.h>

class QFadeLED {
   public:
    QFadeLED(byte pin) : _pin(pin) { pinMode(_pin, OUTPUT); };

    void loop(unsigned long currentMillis) {
         if (currentMillis - _startMillis >= _interval || _startMillis == NULL) {
            _startMillis = currentMillis;

            analogWrite(_pin, _brightness);

            _brightness = _brightness + _fadeAmount;

            if (_brightness <= 0 || _brightness >= 255) _fadeAmount = -_fadeAmount;
        }
    }

    void setFadeDuration(uint16_t fadeDuration) { _interval = fadeDuration * abs(_fadeAmount) / 255; }

   private:
    byte _pin;
    int16_t _brightness = 0;
    byte _fadeAmount = 5;
    byte _interval;
    unsigned long _startMillis;
};