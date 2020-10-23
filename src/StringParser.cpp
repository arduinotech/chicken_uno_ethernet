#include "StringParser.h"

bool StringParser::isNumeric(String string)
{
    for (uint16_t i = 0; i < string.length(); i++) {
        if (string[i] < '0' || string[i] > '9') {
            return false;
        }
    }
    return true;
}

String StringParser::parseTime(int pos, String url)
{
    if (url.length() < pos + 9) {
        return String("");
    }

    String hours = url.substring(pos + 2, pos + 4);
    if (!StringParser::isNumeric(hours)) {
        return String("");
    }
    if (hours.toInt() > 23) {
        return String("");
    }

    String splitter = url.substring(pos + 4, pos + 7);
    if (splitter != String("%3A")) {
        return String("");
    }

    String minutes = url.substring(pos + 7, pos + 9);
    if (!StringParser::isNumeric(minutes)) {
        return String("");
    }

    if (minutes.toInt() > 59) {
        return String("");
    }

    return hours + String(":") + minutes;
}

String StringParser::parseTemp(int pos, String url)
{
    if (url.length() < pos + 3) {
        return String("");
    }

    if ((url[pos + 2] != '-') && (url[pos + 2] < '0' || url[pos + 2] > '9')) {
        return String("");
    }

    if ((url[pos + 2] == '-') && (url.length() < pos + 4)) {
        return String("");
    }

    if ((url[pos + 2] == '-') && (url[pos + 3] < '0' || url[pos + 3] > '9')) {
        return String("");
    }

    String result = String(url[pos + 2]);

    for (int i = pos + 3; i < url.length(); i++) {
        if (url[i] < '0' || url[i] > '9') {
            break;
        }
        result += url[i];
    }
    return result;
}
