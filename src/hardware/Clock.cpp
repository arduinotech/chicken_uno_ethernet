#include "Clock.h"

Clock::Clock()
{
}

void Clock::init()
{
    _rtc.begin();
    if (!_rtc.isrunning()) {
        Serial.println(F("RTC is NOT running!"));
        Serial.print(F("Set date: "));
        Serial.println(__DATE__);
        Serial.print(F("Set time: "));
        Serial.println(__TIME__);
        _rtc.adjust(DateTime(__DATE__, __TIME__));
    }
}

String Clock::getCurrentDateTime()
{
    DateTime now = _rtc.now();
    return dateTimeToString(now);
}

uint16_t Clock::getCurrentTimeInMinutes()
{
    DateTime now = _rtc.now();
    return now.hour() * 60 + now.minute();
}

uint32_t Clock::getCurrentUnixtime()
{
    return _rtc.now().unixtime();
}

uint8_t Clock::getCurrentHour()
{
    DateTime now = _rtc.now();
    return now.hour();
}

String Clock::unixTimeToString(uint32_t unixtime)
{
    return dateTimeToString(DateTime(unixtime));
}

uint16_t Clock::stringTimeToMinutes(String time)
{
    return time.substring(0, 2).toInt() * 60 + time.substring(3, 5).toInt();
}

String Clock::dateTimeToString(DateTime dateTime)
{
    return (dateTime.hour() < 10 ? String("0") : String("")) + String(dateTime.hour()) + String(":")
            + (dateTime.minute() < 10 ? String("0") : String("")) + String(dateTime.minute()) + String(" ")
            + String(dateTime.year()) + String("-")
            + (dateTime.month() < 10 ? String("0") : String("")) + String(dateTime.month()) + String("-")
            + (dateTime.day() < 10 ? String("0") : String("")) + String(dateTime.day());
}
