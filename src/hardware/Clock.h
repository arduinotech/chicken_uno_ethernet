#ifndef Clock_h_
#define Clock_h_

#include <RTClib.h>

class Clock
{
    public:
        Clock();
        void init();
        String getCurrentDateTime();

    private:
        RTC_DS1307 _rtc;
};

#endif