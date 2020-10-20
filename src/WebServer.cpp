#include "WebServer.h"

WebServer::WebServer(uint16_t port)
{
    _server = new EthernetServer(port);
}

void WebServer::init(uint8_t *mac_address, IPAddress local_ip)
{
    Ethernet.begin(mac_address, local_ip);
    _server->begin();
    Serial.print("server is at ");
    Serial.println(Ethernet.localIP());
}

void WebServer::isRequest()
{
    // слушаем порт, ждем запросов
    EthernetClient client = _server->available();
    if (client) {
        Serial.println("Is client request!");
        // запрос должен заканчиваться пустой строкой, так что считываем
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                _request += c;
                // если мы дошли до конца строки (получили символ новой строки) и строка пустая,
                // то http-запрос завершен и можно выдать ответ
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header

                    if (_request.indexOf("GET /favicon.ico") != -1) {
                        Serial.println("Is favicon request - break!");
                        client.println("HTTP/1.1 404 OK");
                        client.println("Connection: close");
                        break;
                    }
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close"); // соединение будет закрыто после получения клиентом ответа
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    client.println(_request);
                    client.println("</html>");
                    break;
                }
                if (c == '\n') {
                    // получен символ новой строки
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // текущая строка продолжается
                    currentLineIsBlank = false;
                }
            }
        }
        _request = "";
        // дадим время браузеру получить все данные
        delay(1);
        // после чего закроем соединение
        client.stop();
    }
}