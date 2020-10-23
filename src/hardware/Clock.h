#ifndef Clock_h_
#define Clock_h_

#include <RTClib.h>

class Clock
{
    public:
        Clock();
        void init();
        String getCurrentDateTime();
        uint16_t getCurrentTimeInMinutes();
        uint32_t getCurrentUnixtime();
        uint8_t getCurrentHour();
        static String unixTimeToString(uint32_t unixtime);
        static uint16_t stringTimeToMinutes(String time);

    private:
        RTC_DS1307 _rtc;
        static String dateTimeToString(DateTime dateTime);
};

#endif