
#include <Arduino.h>

class QPot {
   public:
    QPot(byte pin, uint16_t minValue, uint16_t maxValue) : _pin(pin), _minValue(minValue), _maxValue(maxValue) {
        pinMode(_pin, INPUT);
    }

    void loop(unsigned long currentMillis) {
        if (currentMillis - _startMillis >= _interval || _startMillis == NULL) {
            _startMillis = currentMillis;
            _value = map(analogRead(_pin), 0, 700, _minValue, _maxValue);
        }
    }

    uint16_t value() { return _value; }

   private:
    byte _pin;
    uint16_t _minValue;
    uint16_t _maxValue;
    uint16_t _value;
    uint16_t _interval = 50;
    unsigned long _startMillis;
};