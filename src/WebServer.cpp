#include "WebServer.h"
#include "MemoryFree.h"
#include <RTClib.h>
#include "hardware/Clock.h"

#define DEBUG_MEM(text) Serial.print(F("freeMemory() = ")); Serial.print(freeMemory()); Serial.print(F(" - ")); Serial.println(text);

WebServer::WebServer(uint16_t port)
{
    _server = new EthernetServer(port);
    _request = "";
    _url = "";
}

void WebServer::init(uint8_t *mac_address, IPAddress local_ip, ProcessorFunctionType processorFunction)
{
    Ethernet.begin(mac_address, local_ip);
    _server->begin();
    _processorFunction = processorFunction;
}

void WebServer::listening()
{
    EthernetClient client = _server->available();
    if (client) {
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (_url == String("")) {
                    _request += c;
                }
                String url = parseUrlFromRequest();
                if (url != String("")) {
                    _url = url;
                    _request = "";
                }
                if (c == '\n' && currentLineIsBlank) {
                    DEBUG_MEM(F("listening() before return html"))

                    if (_url == String("")) {
                        return;
                    }

                    HtmlParams htmlParams = _processorFunction(_url);

                    client.println(F("HTTP/1.1 200 OK"));
                    client.println(F("Content-Type: text/html"));
                    client.println(F("Connection: close"));
                    client.println();
                    client.println(F("<!DOCTYPE html>"));
                    client.println(F("<html lang=\"en\">"));
                    client.println(F("<head>"));
                    client.println(F("    <meta charset=\"UTF-8\">"));
                    client.println(F("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"));
                    client.println(F("    <title>Курятник</title>"));
                    client.println(F("</head>"));
                    client.println(F("<body>"));
                    client.print(F("    <p>"));
                    client.print(htmlParams.time);
                    client.println(F("</p>"));
                    client.print(F("    <p>Температура: "));
                    client.print(String(htmlParams.temp).c_str());
                    client.println(F("&deg;C</p>"));
                    client.print(F("    <p>Влажность: "));
                    client.print(String(htmlParams.humi).c_str());
                    client.println(F("%</p>"));
                    client.print(F("    <p>Лампа: "));
                    if (htmlParams.lamp) {
                        client.print(F("ВКЛ"));
                    } else {
                        client.print(F("ВЫКЛ"));
                    }
                    client.println(F("</p>"));
                    client.println(F("    <p>"));
                    client.println(F("        <form>"));
                    if (htmlParams.manual) {
                        client.println(F("            Ручное управление: <input type=\"checkbox\" name=\"m\" checked></br></br>"));
                        if (htmlParams.lamp) {
                            client.println(F("            Лампа: <input type=\"checkbox\" name=\"l\" checked></br></br>"));
                        } else {
                            client.println(F("            Лампа: <input type=\"checkbox\" name=\"l\"></br></br>"));
                        }

                    } else {
                        client.print(F("            Время включения:</br><input type=\"text\" name=\"n\" value=\""));
                        client.print(htmlParams.timeToOn1);
                        client.println(F("\"></br></br>"));
                        client.println(F("            Время выключения:</br><input type=\"text\" name=\"f\" value=\""));
                        client.print(htmlParams.timeToOff1);
                        client.println(F("\"></br></br>"));
                        client.println(F("            Время включения доп:</br><input type=\"text\" name=\"o\" value=\""));
                        client.print(htmlParams.timeToOn2);
                        client.println(F("\"></br></br>"));
                        client.println(F("            Время выключения доп:</br><input type=\"text\" name=\"g\" value=\""));
                        client.print(htmlParams.timeToOff2);
                        client.println(F("\"></br></br>"));
                        client.println(F("            Температура включения:</br><input type=\"text\" name=\"t\" value=\""));
                        client.print(htmlParams.tempToOn);
                        client.println(F("\"></br></br>"));
                        client.println(F("            Ручное управление: <input type=\"checkbox\" name=\"m\"></br></br>"));
                    }
                    client.println(F("            <input type=\"submit\" name=\"s\" value=\"1\">"));
                    client.println(F("        </form>"));
                    client.println(F("    </p>"));
                    client.println(F("    <p>История:</p>"));

                    for (uint8_t i = 0; i < htmlParams.logEventsCount; i++) {
                        client.print(F("    <p>"));
                        client.print(Clock::UnixTimeToString(htmlParams.logEvents[i].unixtime));
                        client.print(F("&nbsp;&nbsp;&nbsp;&nbsp;"));
                        client.print(htmlParams.logEvents[i].temp);
                        client.print(F("&deg;C&nbsp;&nbsp;&nbsp;&nbsp;"));
                        client.print(htmlParams.logEvents[i].humi);
                        client.print(F("%&nbsp;&nbsp;&nbsp;&nbsp;"));
                        if (htmlParams.logEvents[i].lamp) {
                            client.println(F("вкл</p>"));
                        } else {
                            client.println(F("выкл</p>"));
                        }
                    }

                    client.println(F("</body>"));
                    client.println(F("</html>"));
                    DEBUG_MEM(F("listening() after return html"))

                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        }
        _request = "";
        _url = "";
        delay(1);
        client.stop();
    }
}

String WebServer::parseUrlFromRequest()
{
    int posGet = _request.indexOf(F("GET "));
    if (-1 == posGet) {
        return String("");
    }
    int posSpace = _request.indexOf(F(" "), posGet + 4);
    if (-1 == posSpace) {
        return String("");
    }
    return _request.substring(posGet + 4, posSpace);
}