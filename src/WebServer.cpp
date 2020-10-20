#include "WebServer.h"
#include "MemoryFree.h"

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
                    String response;
                    //String url = parseUrlFromRequest();
    Serial.print("1 freeMemory()=");
    Serial.println(freeMemory());
                    if (String("") == _url) {
                        response = "";
                    } else {
                        response = _processorFunction(_url);
                    }
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    client.println(response);
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