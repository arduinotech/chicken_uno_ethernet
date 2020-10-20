#ifndef WebServer_h_
#define WebServer_h_

#include <Arduino.h>
#include <Ethernet.h>

class WebServer
{
    public:
        WebServer(uint16_t port);
        void init(uint8_t *mac_address, IPAddress local_ip);
        void isRequest();

    private:
        EthernetServer *_server;
        String _request;
};

#endif