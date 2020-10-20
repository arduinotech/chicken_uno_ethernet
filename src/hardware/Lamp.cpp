#include "Lamp.h"

Lamp::Lamp(uint8_t pin)
{
    _pin = pin;
}

void Lamp::init()
{
    Serial.println("Initing lamp...");
    pinMode(_pin, OUTPUT);
    off();
}

bool Lamp::isOn()
{
    Serial.println("Request lamp status");
    return _curState;
}

void Lamp::on()
{
    Serial.println("Lamp on");
    digitalWrite(_pin, HIGH);
    _curState = true;
}

void Lamp::off()
{
    Serial.println("Lamp off");
    digitalWrite(_pin, LOW);
    _curState = false;
}
