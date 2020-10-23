#ifndef SettingsStorage_h_
#define SettingsStorage_h_

#include <Arduino.h>

class SettingsStorage
{
    public:
        SettingsStorage();
        void init();
        bool getManual();
        void setManual(bool manual);
        bool getLamp();
        void setLamp(bool lamp);
        String getTimeToOn1();
        void setTimeToOn1(String timeToOn1);
        String getTimeToOff1();
        void setTimeToOff1(String timeToOff1);
        String getTimeToOn2();
        void setTimeToOn2(String timeToOn2);
        String getTimeToOff2();
        void setTimeToOff2(String timeToOff2);
        String getTempToOn();
        void setTempToOn(String tempToOn);

    private:
        bool _manual;
        bool _lamp;
        String _timeToOn1;
        String _timeToOff1;
        String _timeToOn2;
        String _timeToOff2;
        String _tempToOn;

        bool readManual();
        void writeManual(bool manual);
        bool readLamp();
        void writeLamp(bool lamp);
        String readTimeToOn1();
        void writeTimeToOn1(String timeToOn1);
        String readTimeToOff1();
        void writeTimeToOff1(String timeToOff1);
        String readTimeToOn2();
        void writeTimeToOn2(String timeToOn2);
        String readTimeToOff2();
        void writeTimeToOff2(String timeToOff2);
        String readTempToOn();
        void writeTempToOn(String tempToOn);

        void writeTime(String time, int idx);
        String readTime(int idx);
        void writeBool(bool val, int idx);
        bool readBool(int idx);
        void writeTemp(String temp, int idx);
        String readTemp(int idx);
};

#endif