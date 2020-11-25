#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>

#include "WebServer.h"
#include "Config.h"
#include "hardware/SensorDHT22.h"
#include "hardware/Lamp.h"
#include "hardware/Clock.h"
#include "StringParser.h"
#include "SettingsStorage.h"
#include "GyverWDT.h"
#include "MemoryFree.h"

WebServer webServer(PORT);
SensorDHT22 dht(DHT22_PIN);
Lamp lamp(LAMP_PIN);
Clock clock;
SettingsStorage settingsStorage;

LogEvent logEvents[LOG_SIZE];
uint8_t logEventsCount = 0;

void lampOnOrOffIfNeed()
{
    if (settingsStorage.getManual()) {
        if (settingsStorage.getLamp() && !lamp.isOn()) {
            lamp.on();
        } else if (!settingsStorage.getLamp() && lamp.isOn()) {
            lamp.off();
        }
        return;
    }

    uint16_t currentTime = clock.getCurrentTimeInMinutes();
    if ((settingsStorage.getTempToOn() == String("")) || (dht.getTemp() <= settingsStorage.getTempToOn().toInt())) {
        if ((settingsStorage.getTimeToOn1() != String("")) && (settingsStorage.getTimeToOff1() != String(""))) {
            uint16_t timeToOn = Clock::stringTimeToMinutes(settingsStorage.getTimeToOn1());
            uint16_t timeToOff = Clock::stringTimeToMinutes(settingsStorage.getTimeToOff1());
            if ((timeToOn <= currentTime) && (currentTime < timeToOff)) {
                if (!lamp.isOn()) {
                    lamp.on();
                }
                return;
            }
        }

        if ((settingsStorage.getTimeToOn2() != String("")) && (settingsStorage.getTimeToOff2() != String(""))) {
            uint16_t timeToOn = Clock::stringTimeToMinutes(settingsStorage.getTimeToOn2());
            uint16_t timeToOff = Clock::stringTimeToMinutes(settingsStorage.getTimeToOff2());
            if ((timeToOn <= currentTime) && (currentTime < timeToOff)) {
                if (!lamp.isOn()) {
                    lamp.on();
                }
                return;
            }
        }
    }

    if ((settingsStorage.getTimeToOn1() != String(""))
        && (settingsStorage.getTimeToOff1() != String(""))
        && (settingsStorage.getTimeToOff2() == String(""))
        && (settingsStorage.getTimeToOff2() == String(""))) {
        uint16_t timeToOn = Clock::stringTimeToMinutes(settingsStorage.getTimeToOn1());
        uint16_t timeToOff = Clock::stringTimeToMinutes(settingsStorage.getTimeToOff1());
        if ((currentTime < timeToOn) || (currentTime >= timeToOff)) {
            if (lamp.isOn()) {
                lamp.off();
            }
        }
        return;
    }

    if ((settingsStorage.getTimeToOn1() != String(""))
        && (settingsStorage.getTimeToOff1() != String(""))
        && (settingsStorage.getTimeToOff2() != String(""))
        && (settingsStorage.getTimeToOff2() != String(""))) {
        uint16_t timeToOn1 = Clock::stringTimeToMinutes(settingsStorage.getTimeToOn1());
        uint16_t timeToOff1 = Clock::stringTimeToMinutes(settingsStorage.getTimeToOff1());
        uint16_t timeToOn2 = Clock::stringTimeToMinutes(settingsStorage.getTimeToOn2());
        uint16_t timeToOff2 = Clock::stringTimeToMinutes(settingsStorage.getTimeToOff2());
        if (((currentTime < timeToOn1) && (currentTime < timeToOn2)) || ((currentTime >= timeToOff1) && (currentTime >= timeToOff2))) {
            if (lamp.isOn()) {
                lamp.off();
            }
        }
        return;
    }

    if (lamp.isOn()) {
        lamp.off();
    }
}

HtmlParams processor(String url)
{
    // Serial.print(F("Request: "));
    // Serial.println(url);

    if (String(F("/kill")) == url) {
        while(true) {
            delay(10);
        }
    }

    if (String(F("/")) == url) {
        return {clock.getCurrentDateTime(),
                dht.getTemp(),
                dht.getHumi(),
                lamp.isOn(),
                settingsStorage.getManual(),
                settingsStorage.getTimeToOn1(),
                settingsStorage.getTimeToOff1(),
                settingsStorage.getTimeToOn2(),
                settingsStorage.getTimeToOff2(),
                settingsStorage.getTempToOn(),
                logEvents,
                logEventsCount};
    }

    if (!settingsStorage.getManual() && (url.indexOf(F("m=on")) != -1)) {
        settingsStorage.setManual(true);
    }

    if (settingsStorage.getManual() && (url.indexOf(F("m=on")) == -1) && (url.indexOf(F("s=")) != -1)) {
        settingsStorage.setManual(false);
        settingsStorage.setLamp(false);
    }

    if (settingsStorage.getManual() && !lamp.isOn() && (url.indexOf(F("l=on")) != -1)) {
        lamp.on();
        settingsStorage.setLamp(true);
    }

    if (settingsStorage.getManual() && (url.indexOf(F("m=on")) != -1) && (url.indexOf(F("l=on")) == -1) && (url.indexOf(F("f=")) == -1)) {
        lamp.off();
        settingsStorage.setLamp(false);
    }

    int pos;

    pos = url.indexOf(F("n="));
    if (pos != -1) {
        settingsStorage.setTimeToOn1(StringParser::parseTime(pos, url));
    }

    pos = url.indexOf(F("f="));
    if (pos != -1) {
        settingsStorage.setTimeToOff1(StringParser::parseTime(pos, url));
    }

    pos = url.indexOf(F("o="));
    if (pos != -1) {
        settingsStorage.setTimeToOn2(StringParser::parseTime(pos, url));
    }

    pos = url.indexOf(F("g="));
    if (pos != -1) {
        settingsStorage.setTimeToOff2(StringParser::parseTime(pos, url));
    }

    pos = url.indexOf(F("t="));
    if (pos != -1) {
        settingsStorage.setTempToOn(StringParser::parseTemp(pos, url));
    }

    pos = url.indexOf(F("d="));
    if (pos != -1) {
        String newDateTime = StringParser::parseDateTime(pos, url);
        if (newDateTime != String("")) {
            clock.setCurrentDateTime(newDateTime);
        }
    }

    lampOnOrOffIfNeed();

    return {clock.getCurrentDateTime(),
            dht.getTemp(),
            dht.getHumi(),
            lamp.isOn(),
            settingsStorage.getManual(),
            settingsStorage.getTimeToOn1(),
            settingsStorage.getTimeToOff1(),
            settingsStorage.getTimeToOn2(),
            settingsStorage.getTimeToOff2(),
            settingsStorage.getTempToOn(),
            logEvents,
            logEventsCount};
}

void setup()
{
    // Serial.begin(115200);

    // hardware
    dht.init();
    lamp.init();
    clock.init();
    pinMode(WORK_INDICATOR_PIN, OUTPUT);

    settingsStorage.init();

    if (settingsStorage.getManual() && settingsStorage.getLamp()) {
        lamp.on();
    }

    // network
    byte mac[] = MAC;
    byte ip[] = IP;
    webServer.init(mac, ip, processor);

    Watchdog.enable(RESET_MODE, WDT_PRESCALER_512);
}

void loop()
{
    uint32_t now = millis();
    static uint32_t lampOnOffIfNeedLastCall = 0;
    static uint8_t lastHourSaveData = 1;
    static bool workIndicatorState = true;
    static uint32_t workIndicatorLastChangeTime = 0;

    if (lampOnOffIfNeedLastCall > now) {
        lampOnOffIfNeedLastCall = 0;
    }

    if (workIndicatorLastChangeTime > now) {
        workIndicatorLastChangeTime = 0;
    }

    if (now > (workIndicatorLastChangeTime + WORK_INDICATOR_INTERVAL)) {
        workIndicatorState = !workIndicatorState;
        if (workIndicatorState) {
            digitalWrite(WORK_INDICATOR_PIN, HIGH);
        } else {
            digitalWrite(WORK_INDICATOR_PIN, LOW);
        }
        workIndicatorLastChangeTime = now;
    }

    if ((now - lampOnOffIfNeedLastCall) > CHECK_INTERVAL) {
        lampOnOrOffIfNeed();
        uint8_t hour = clock.getCurrentHour();
        if ((hour % 2 == 0) && (hour != lastHourSaveData)) {
            LogEvent newEvent = {clock.getCurrentUnixtime(), dht.getTemp(), dht.getHumi(), lamp.isOn(), freeMemory()};
            if (logEventsCount < LOG_SIZE) {
                logEvents[logEventsCount] = newEvent;
                logEventsCount++;
            } else {
                for (int i = 0; i < LOG_SIZE - 1; i++) {
                    logEvents[i] = logEvents[i + 1];
                }
                logEvents[LOG_SIZE - 1] = newEvent;
            }
            lastHourSaveData = hour;
        }
    }

    webServer.listening();
    Watchdog.reset();
}