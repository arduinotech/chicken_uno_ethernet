#ifndef StringParser_h_
#define StringParser_h_

#include <Arduino.h>

class StringParser
{
    public:
        static bool isNumeric(String string);
        static String parseTime(int pos, String url);
        static String parseTemp(int pos, String url);
};

#endif