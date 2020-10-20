#ifndef WebServer_h_
#define WebServer_h_

#include <Arduino.h>
#include <Ethernet.h>

typedef String (*ProcessorFunctionType)(String);

class WebServer
{
    public:
        WebServer(uint16_t port);
        void init(uint8_t *mac_address, IPAddress local_ip, ProcessorFunctionType processorFunction);
        void listening();

    private:
        EthernetServer *_server;
        ProcessorFunctionType _processorFunction;
        String parseUrlFromRequest();
        String _request;
        String _url;
};

#endif