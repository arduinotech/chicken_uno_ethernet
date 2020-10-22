#ifndef WebServer_h_
#define WebServer_h_

#include <Arduino.h>
#include <Ethernet.h>

struct HtmlParams {
    String time;
    int8_t temp;
    int8_t humi;
    bool lamp;
};

typedef HtmlParams (*ProcessorFunctionType)(String);

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