
#include <Arduino.h>

class QPot {
   public:
    QPot(byte pin, uint16_t minValue, uint16_t maxValue) : _pin(pin), _minValue(minValue), _maxValue(maxValue) {
        pinMode(_pin, INPUT);
    }

    uint16_t read() {
        _measuredValue = analogRead(_pin);

        if (_measuredValue > 700) _measuredValue = 700;

        if (abs(_measuredValue - _previousMeasuredValue) > 50) {
            _previousMeasuredValue = _measuredValue;
            _value = map(_measuredValue, 0, 700, _minValue, _maxValue);
        }

        return _value;
    }

   private:
    byte _pin;
    uint16_t _minValue;
    uint16_t _maxValue;
    uint16_t _value = 0;
    uint16_t _measuredValue;
    uint16_t _previousMeasuredValue = 0;
};