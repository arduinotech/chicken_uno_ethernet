#include <Arduino.h>
#include <SPI.h>

#include "WebServer.h"
#include "Config.h"

WebServer webServer(PORT);

void setup()
{
    Serial.begin(115200);
    byte mac[] = MAC;
    byte ip[] = IP;
    webServer.init(mac, ip);
}

void loop()
{
    webServer.isRequest();
}