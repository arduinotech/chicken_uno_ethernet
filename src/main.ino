#include <Arduino.h>
#include <SPI.h>

#include "WebServer.h"
#include "Config.h"

WebServer webServer(PORT);

Response processor(String url)
{
    Serial.println("Url:");
    Serial.println(url);
    return {false, "MAIN"};
}

void setup()
{
    Serial.begin(115200);
    byte mac[] = MAC;
    byte ip[] = IP;
    webServer.init(mac, ip, processor);
}

void loop()
{
    webServer.listening();
}