#ifndef WebServer_h_
#define WebServer_h_

#include <Arduino.h>
#include <Ethernet.h>

#define NO_REQUEST 0
#define NORMAL_REQUEST 1
#define BAD_REQUEST 2

struct LogEvent {
    uint32_t unixtime;
    int8_t temp;
    int8_t humi;
    bool lamp;
    int freeMemory;
};

struct HtmlParams {
    String time;
    int8_t temp;
    int8_t humi;
    bool lamp;
    bool manual;
    String timeToOn1;
    String timeToOff1;
    String timeToOn2;
    String timeToOff2;
    String tempToOn;
    LogEvent *logEvents;
    uint8_t logEventsCount;
};

typedef HtmlParams (*ProcessorFunctionType)(String);

class WebServer
{
    public:
        WebServer(uint16_t port);
        void init(uint8_t *mac_address, IPAddress local_ip, ProcessorFunctionType processorFunction);
        uint8_t listening();

    private:
        EthernetServer *_server;
        ProcessorFunctionType _processorFunction;
        String parseUrlFromRequest();
        String _request;
        String _url;
};

#endif