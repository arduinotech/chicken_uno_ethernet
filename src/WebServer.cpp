#include "WebServer.h"

WebServer::WebServer(uint16_t port)
{
    _server = new EthernetServer(port);
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
                _request += c;
                if (c == '\n' && currentLineIsBlank) {
                    String response;
                    String url = parseUrlFromRequest();
                    if (String("") == url) {
                        response = "";
                    } else {
                        response = _processorFunction(url);
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
        delay(1);
        client.stop();
    }
}

String WebServer::parseUrlFromRequest()
{
    int posGet = _request.indexOf("GET ");
    if (-1 == posGet) {
        return "";
    }
    int posSpace = _request.indexOf(" ", posGet + 4);
    if (-1 == posSpace) {
        return "";
    }
    return _request.substring(posGet + 4, posSpace);
}