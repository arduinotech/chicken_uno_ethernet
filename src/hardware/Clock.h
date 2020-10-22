#ifndef Clock_h_
#define Clock_h_

#include <RTClib.h>

class Clock
{
    public:
        Clock();
        void init();
        String getCurrentDateTime();
        uint32_t getCurrentUnixtime();
        static String UnixTimeToString(uint32_t unixtime);

    private:
        RTC_DS1307 _rtc;
        static String DateTimeToString(DateTime dateTime);
};

#endif