#include "SettingsStorage.h"
#include <EEPROM.h>

#define EMPTY_BYTE -100

#define MANUAL_IDX 10
#define LAMP_IDX 11
#define TIME_TO_ON_1_IDX 12
#define TIME_TO_OFF_1_IDX 14
#define TIME_TO_ON_2_IDX 16
#define TIME_TO_OFF_2_IDX 18
#define TEMP_TO_ON_IDX 20

// ============================ PUBLIC ============================

SettingsStorage::SettingsStorage()
{
}

void SettingsStorage::init()
{
    // writeBool(false, MANUAL_IDX);
    // writeBool(false, LAMP_IDX);
    // writeTime(String("06:00"), TIME_TO_ON_1_IDX);
    // writeTime(String("10:00"), TIME_TO_OFF_1_IDX);
    // writeTime(String("18:00"), TIME_TO_ON_2_IDX);
    // writeTime(String("20:00"), TIME_TO_OFF_2_IDX);
    // writeTemp(String("6"), TEMP_TO_ON_IDX);
    _manual = readManual();
    // Serial.print("_manual = ");
    // Serial.println(_manual ? "true" : "false");
    _lamp = readLamp();
    // Serial.print("_lamp = ");
    // Serial.println(_lamp ? "true" : "false");
    _timeToOn1 = readTimeToOn1();
    _timeToOff1 = readTimeToOff1();
    _timeToOn2 = readTimeToOn2();
    _timeToOff2 = readTimeToOff2();
    _tempToOn = readTempToOn();
}

bool SettingsStorage::getManual()
{
    return _manual;
}

void SettingsStorage::setManual(bool manual)
{
    if (_manual != manual) {
        writeManual(manual);
        _manual = manual;
    }
}

bool SettingsStorage::getLamp()
{
    return _lamp;
}

void SettingsStorage::setLamp(bool lamp)
{
    if (_lamp != lamp) {
        writeLamp(lamp);
        _lamp = lamp;
    }
}

String SettingsStorage::getTimeToOn1()
{
    return _timeToOn1;
}

void SettingsStorage::setTimeToOn1(String timeToOn1)
{
    if (_timeToOn1 != timeToOn1) {
        writeTimeToOn1(timeToOn1);
        _timeToOn1 = timeToOn1;
    }
}

String SettingsStorage::getTimeToOff1()
{
    return _timeToOff1;
}

void SettingsStorage::setTimeToOff1(String timeToOff1)
{
    if (_timeToOff1 != timeToOff1) {
        writeTimeToOff1(timeToOff1);
        _timeToOff1 = timeToOff1;
    }
}

String SettingsStorage::getTimeToOn2()
{
    return _timeToOn2;
}

void SettingsStorage::setTimeToOn2(String timeToOn2)
{
    if (_timeToOn2 != timeToOn2) {
        writeTimeToOn2(timeToOn2);
        _timeToOn2 = timeToOn2;
    }
}

String SettingsStorage::getTimeToOff2()
{
    return _timeToOff2;
}

void SettingsStorage::setTimeToOff2(String timeToOff2)
{
    if (_timeToOff2 != timeToOff2) {
        writeTimeToOff2(timeToOff2);
        _timeToOff2 = timeToOff2;
    }
}

String SettingsStorage::getTempToOn()
{
    return _tempToOn;
}

void SettingsStorage::setTempToOn(String tempToOn)
{
    if (_tempToOn != tempToOn) {
        writeTempToOn(tempToOn);
        _tempToOn = tempToOn;
    }
}

// ============================ PRIVATE ============================

bool SettingsStorage::readManual()
{
    return readBool(MANUAL_IDX);
}

void SettingsStorage::writeManual(bool manual)
{
    writeBool(manual, MANUAL_IDX);
}

bool SettingsStorage::readLamp()
{
    return readBool(LAMP_IDX);
}

void SettingsStorage::writeLamp(bool lamp)
{
    writeBool(lamp, LAMP_IDX);
}

String SettingsStorage::readTimeToOn1()
{
    return readTime(TIME_TO_ON_1_IDX);
}

void SettingsStorage::writeTimeToOn1(String timeToOn1)
{
    writeTime(timeToOn1, TIME_TO_ON_1_IDX);
}

String SettingsStorage::readTimeToOff1()
{
    return readTime(TIME_TO_OFF_1_IDX);
}

void SettingsStorage::writeTimeToOff1(String timeToOff1)
{
    writeTime(timeToOff1, TIME_TO_OFF_1_IDX);
}

String SettingsStorage::readTimeToOn2()
{
    return readTime(TIME_TO_ON_2_IDX);
}

void SettingsStorage::writeTimeToOn2(String timeToOn2)
{
    writeTime(timeToOn2, TIME_TO_ON_2_IDX);
}

String SettingsStorage::readTimeToOff2()
{
    return readTime(TIME_TO_OFF_2_IDX);
}

void SettingsStorage::writeTimeToOff2(String timeToOff2)
{
    writeTime(timeToOff2, TIME_TO_OFF_2_IDX);
}

String SettingsStorage::readTempToOn()
{
    return readTemp(TEMP_TO_ON_IDX);
}

void SettingsStorage::writeTempToOn(String tempToOn)
{
    writeTemp(tempToOn, TEMP_TO_ON_IDX);
}

void SettingsStorage::writeTime(String time, int idx)
{
    if (time == String("")) {
        EEPROM.write(idx, EMPTY_BYTE);
        EEPROM.write(idx + 1, EMPTY_BYTE);
        return;
    }
    int8_t val = time.substring(0, 2).toInt();
    EEPROM.write(idx, val);
    val = time.substring(3, 5).toInt();
    EEPROM.write(idx + 1, val);
}

String SettingsStorage::readTime(int idx)
{
    String time = String("");
    int8_t val = EEPROM.read(idx);
    if (val == EMPTY_BYTE) {
        return String("");
    }

    time += (val < 10) ? String("0") + String(val) : String(val);

    time += String(":");

    val = EEPROM.read(idx + 1);
    time += (val < 10) ? String("0") + String(val) : String(val);
    return time;
}

void SettingsStorage::writeBool(bool val, int idx)
{
    int8_t valByte = val ? 1 : 0;
    EEPROM.write(idx, valByte);
}

bool SettingsStorage::readBool(int idx)
{
    int8_t val = EEPROM.read(idx);
    return val == 1;
}

void SettingsStorage::writeTemp(String temp, int idx)
{
    if (temp == String("")) {
        EEPROM.write(idx, EMPTY_BYTE);
        return;
    }
    int8_t val = temp.toInt();
    EEPROM.write(idx, val);
}

String SettingsStorage::readTemp(int idx)
{
    int8_t val = EEPROM.read(idx);
    return (val == EMPTY_BYTE) ? String("") : String(val);
}
