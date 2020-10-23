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

LogEvent logEvents[12];
uint8_t logEventsCount = 0;

bool manual = false;
String timeToOn1 = "06:00";
String timeToOff1 = "10:00";
String timeToOn2 = "18:00";
String timeToOff2 = "20:00";
String tempToOn = "6";

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
                manual,
                timeToOn1,
                timeToOff1,
                timeToOn2,
                timeToOff2,
                tempToOn,
                logEvents,
                logEventsCount};
    }

    if (!manual && (url.indexOf(F("m=on")) != -1)) {
        manual = true;
    }

    if (manual && (url.indexOf(F("m=on")) == -1)) {
        manual = false;
        // TODO тут лампу зажечь или потушить - по логике времени и температуры
    }

    if (manual && !lamp.isOn() && (url.indexOf(F("l=on")) != -1)) {
        lamp.on();
    }

    if (manual && (url.indexOf(F("m=on")) != -1) && (url.indexOf(F("l=on")) == -1) && (url.indexOf(F("f=")) == -1)) {
        lamp.off();
    }

    int pos;

    pos = url.indexOf(F("n="));
    if (pos != -1) {
        String newTimeToOn1 = StringParser::parseTime(pos, url);
        if (newTimeToOn1 != timeToOn1) {
            timeToOn1 = newTimeToOn1;
        }
    }

    pos = url.indexOf(F("f="));
    if (pos != -1) {
        String newTimeToOff1 = StringParser::parseTime(pos, url);
        if (newTimeToOff1 != timeToOff1) {
            timeToOff1 = newTimeToOff1;
        }
    }

    pos = url.indexOf(F("o="));
    if (pos != -1) {
        String newTimeToOn2 = StringParser::parseTime(pos, url);
        if (newTimeToOn2 != timeToOn2) {
            timeToOn2 = newTimeToOn2;
        }
    }

    pos = url.indexOf(F("g="));
    if (pos != -1) {
        String newTimeToOff2 = StringParser::parseTime(pos, url);
        if (newTimeToOff2 != timeToOff2) {
            timeToOff2 = newTimeToOff2;
        }
    }

    pos = url.indexOf(F("t="));
    if (pos != -1) {
        String newTempToOn = StringParser::parseTemp(pos, url);
        if (newTempToOn != tempToOn) {
            tempToOn = newTempToOn;
        }
    }

    DEBUG_MEM(F("processor end"))
    return {clock.getCurrentDateTime(),
            dht.getTemp(),
            dht.getHumi(),
            lamp.isOn(),
            manual,
            timeToOn1,
            timeToOff1,
            timeToOn2,
            timeToOff2,
            tempToOn,
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
    webServer.listening();
}