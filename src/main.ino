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

// TODO remove for production
#include "MemoryFree.h"

#define DEBUG_MEM(text) Serial.print(F("freeMemory() = ")); Serial.print(freeMemory()); Serial.print(F(" - ")); Serial.println(text);

WebServer webServer(PORT);
SensorDHT22 dht(DHT22_PIN);
Lamp lamp(LAMP_PIN);
Clock clock;
SettingsStorage settingsStorage;

LogEvent logEvents[12];
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

    if ((settingsStorage.getTempToOn() != String("")) && (dht.getTemp() > settingsStorage.getTempToOn().toInt())) {
        if (lamp.isOn()) {
            lamp.off();
        }
        return;
    }

    uint16_t currentTime = clock.getCurrentTimeInMinutes();
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

    if (lamp.isOn()) {
        lamp.off();
    }
}

HtmlParams processor(String url)
{
    DEBUG_MEM(F("processor begin"))

    Serial.print(F("Request: "));
    Serial.println(url);

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

    DEBUG_MEM(F("processor end"))
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
    Serial.begin(115200);

    DEBUG_MEM(F("setup begin"))

    // hardware
    dht.init();
    lamp.init();
    clock.init();

    settingsStorage.init();

    if (settingsStorage.getManual() && settingsStorage.getLamp()) {
        lamp.on();
    }

    DEBUG_MEM(F("setup hardware inited"))

    // network
    byte mac[] = MAC;
    byte ip[] = IP;
    webServer.init(mac, ip, processor);

    logEvents[0] = {clock.getCurrentUnixtime(), 6, 79, true};
    logEventsCount++;

    logEvents[1] = {clock.getCurrentUnixtime(), 7, 77, true};
    logEventsCount++;

    logEvents[2] = {clock.getCurrentUnixtime(), 12, 61, false};
    logEventsCount++;
    DEBUG_MEM(F("setup end"))
}

void loop()
{
    uint32_t now = millis();
    static uint32_t lampOnOffIfNeedLastCall = 0;

    if (lampOnOffIfNeedLastCall > now) {
        lampOnOffIfNeedLastCall = 0;
    }

    if ((now - lampOnOffIfNeedLastCall) > LAMP_CHECK_INTERVAL) {
        lampOnOrOffIfNeed();
    }

    webServer.listening();
}