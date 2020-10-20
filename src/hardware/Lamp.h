#ifndef Lamp_h_
#define Lamp_h_

#include <Arduino.h>

class Lamp
{
    public:
        Lamp(uint8_t pin);
        void init();
        bool isOn();
        void on();
        void off();

    private:
        uint8_t _pin;
        bool _curState;
};

#endif