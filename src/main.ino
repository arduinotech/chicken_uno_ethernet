#include <Arduino.h>
#include <SPI.h>

#include "WebServer.h"
#include "Config.h"
#include "hardware/SensorDHT22.h"
#include "hardware/Lamp.h"
#include "hardware/Clock.h"

#include "MemoryFree.h"

WebServer webServer(PORT);
SensorDHT22 dht(DHT22_PIN);
Lamp lamp(LAMP_PIN);
Clock clock;

String processor(String url)
{
    Serial.println("processor begin");
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());

    Serial.print("Request: ");
    Serial.println(url);

    if (String("/") == url) {
        Serial.println("111");
        String html = String(R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Курятник</title>
</head>
<body>
<p>Температура:%TEMP%</p>
</body>
</html>)rawliteral");

//         String html = String(R"rawliteral(<!DOCTYPE html>
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <title>Chicken</title>
// </head>
// <body>
// <p>Temperature:%TEMP%</p>
// </body>
// </html>)rawliteral");
// String html = String("<p>Temperature:%TEMP%</p>");
        // Serial.println("222");
        // Serial.println(html);
        // Serial.println("333");
        // Serial.println("444");
        String from = String("%TEMP%");
        String to = String(dht.getTemp());
        html.replace(from, to);
        // Serial.println("555");
        // Serial.println(html);
        return html;
    }

    if (String("/temp") == url) {
        return String(dht.getTemp());
    }
    if (String("/humi") == url) {
        return String(dht.getHumi());
    }
    if (String("/lamp") == url) {
        return String(lamp.isOn() ? 1 : 0);
    }
    if (String("/time") == url) {
        return clock.getCurrentDateTime();
    }

    if (String("/lamp-on") == url) {
        lamp.on();
        return String(lamp.isOn() ? 1 : 0);
    }

    if (String("/lamp-off") == url) {
        lamp.off();
        return String(lamp.isOn() ? 1 : 0);
    }
    return String("");
}

void setup()
{
    Serial.begin(115200);

    Serial.print("freeMemory()=");
    Serial.println(freeMemory());




//     String html = String(R"rawliteral(<!DOCTYPE html>
// <html lang="en">
// <head>
//     <meta charset="UTF-8">
//     <title>Курятник</title>
// </head>
// <body>
// <p>Температура:%TEMP%</p>
// </body>
// </html>)rawliteral");
//     Serial.println("222");
//     Serial.println(html);
//     html.replace("%TEMP%", String(dht.getTemp()));
//     Serial.println(html);




    // hardware
    dht.init();
    lamp.init();
    clock.init();

    Serial.print("freeMemory()=");
    Serial.println(freeMemory());

    // network
    byte mac[] = MAC;
    byte ip[] = IP;
    webServer.init(mac, ip, processor);

    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
}

void loop()
{
    webServer.listening();
}