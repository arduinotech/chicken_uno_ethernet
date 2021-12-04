#include "Lamp.h"

Lamp::Lamp(uint8_t pin)
{
    _pin = pin;
}

void Lamp::init()
{
    pinMode(_pin, OUTPUT);
    off();
}

bool Lamp::isOn()
{
    return _curState;
}

void Lamp::on()
{
    digitalWrite(_pin, LOW);
    _curState = true;
}

void Lamp::off()
{
    digitalWrite(_pin, HIGH);
    _curState = false;
}
