#include "WebServer.h"
#include "MemoryFree.h"

#define DEBUG_MEM(text) Serial.print("freeMemory() = "); Serial.print(freeMemory()); Serial.print(" - "); Serial.println(text);

WebServer::WebServer(uint16_t port)
{
    _server = new EthernetServer(port);
    _request = "";
    _url = "";
}

void WebServer::init(uint8_t *mac_address, IPAddress local_ip, ProcessorFunctionType processorFunction)
{
    Serial.println("Initing webServer...");
    Ethernet.begin(mac_address, local_ip);
    _server->begin();
    _processorFunction = processorFunction;
    Serial.println("OK");
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
                    DEBUG_MEM("listening() before return html")

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
                    client.println(F("            Время включения: <input type=\"text\" name=\"time_lamp_on_1\"></br></br>"));
                    client.println(F("            Время выключения: <input type=\"text\" name=\"time_lamp_off_1\"></br></br>"));
                    client.println(F("            Время включения доп: <input type=\"text\" name=\"time_lamp_on_2\"></br></br>"));
                    client.println(F("            Время выключения доп: <input type=\"text\" name=\"time_lamp_off_2\"></br></br>"));
                    client.println(F("            Температура включения: <input type=\"text\" name=\"temp_to_on\"></br></br>"));
                    client.println(F("            <input type=\"submit\" name=\"save\"></br></br>"));
                    client.println(F("            Ручное управление: <input type=\"checkbox\" name=\"manual_control\">"));
                    client.println(F("        </form>"));
                    client.println(F("    </p>"));
                    client.println(F("</body>"));
                    client.println(F("</html>"));
                    DEBUG_MEM("listening() after return html")

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
    int posGet = _request.indexOf("GET ");
    if (-1 == posGet) {
        return String("");
    }
    int posSpace = _request.indexOf(" ", posGet + 4);
    if (-1 == posSpace) {
        return String("");
    }
    return _request.substring(posGet + 4, posSpace);
}