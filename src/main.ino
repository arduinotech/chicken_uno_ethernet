#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>

#include "WebServer.h"
#include "Config.h"
#include "hardware/SensorDHT22.h"
#include "hardware/Lamp.h"
#include "hardware/Clock.h"

#include "MemoryFree.h"

#define DEBUG_MEM(text) Serial.print("freeMemory() = "); Serial.print(freeMemory()); Serial.print(" - "); Serial.println(text);

WebServer webServer(PORT);
SensorDHT22 dht(DHT22_PIN);
Lamp lamp(LAMP_PIN);
Clock clock;

HtmlParams processor(String url)
{
    DEBUG_MEM("processor begin")

    Serial.print("Request: ");
    Serial.println(url);

    if (String("/") == url) {
    }


    if (String("/lamp-on") == url) {
        lamp.on();
    }

    if (String("/lamp-off") == url) {
        lamp.off();
    }

    DEBUG_MEM("processor end")
    return {clock.getCurrentDateTime(), dht.getTemp(), dht.getHumi(), lamp.isOn()};
}

void setup()
{
    Serial.begin(115200);

    DEBUG_MEM("setup begin")

    // hardware
    dht.init();
    lamp.init();
    clock.init();

    DEBUG_MEM("setup hardware inited")

    // network
    byte mac[] = MAC;
    byte ip[] = IP;
    webServer.init(mac, ip, processor);

    DEBUG_MEM("setup end")
}

void loop()
{
    webServer.listening();
}