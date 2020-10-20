#include "Clock.h"

Clock::Clock()
{
}

void Clock::init()
{
    Serial.println("Initing clock...");
    _rtc.begin();
    if (!_rtc.isrunning()) {
        Serial.println("RTC is NOT running!");
        Serial.print("Set date: ");
        Serial.println(__DATE__);
        Serial.print("Set time: ");
        Serial.println(__TIME__);
        _rtc.adjust(DateTime(__DATE__, __TIME__));
    } else {
        Serial.println("Clock ok");
        Serial.println(getCurrentDateTime());
    }
}

String Clock::getCurrentDateTime()
{
  DateTime now = _rtc.now();
  return (now.hour() < 10 ? String("0") : String("")) + String(now.hour()) + String(":")
         + (now.minute() < 10 ? String("0") : String("")) + String(now.minute()) + String(" ")
         + String(now.year()) + String("-")
         + (now.month() < 10 ? String("0") : String("")) + String(now.month()) + String("-")
         + (now.day() < 10 ? String("0") : String("")) + String(now.day());
}