#include <Arduino.h>

class QFadeLED {
   public:
    QFadeLED(byte pin) : _pin(pin) { pinMode(_pin, OUTPUT); };

    void loop(unsigned long currentMillis) {
        if (currentMillis - _startMillis >= _interval || _startMillis == 0) {
            _startMillis = currentMillis;

            analogWrite(_pin, _brightness);

            _brightness = _isFadingIn ? _brightness + _fadeAmount : _brightness - _fadeAmount;

            if (_brightness <= 0) {
                _isFadingIn = true;
                _brightness = 0;
            } else if (_brightness >= 255) {
                _isFadingIn = false;
                _brightness = 255;
            }
        }
    }

    void setFadeDuration(uint16_t fadeDuration) { _interval = fadeDuration * _fadeAmount / 255; }

   private:
    byte _pin;
    int16_t _brightness = 0;
    byte _fadeAmount = 5;
    bool _isFadingIn = true;
    byte _interval;
    unsigned long _startMillis = 0;
};