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

String StringParser::parseDateTime(int pos, String url)
{
    // d=18%3A55+2020-11-08

    if (url.length() < pos + 20) {
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

    splitter = url.substring(pos + 9, pos + 10);
    if (splitter != String("+")) {
        return String("");
    }

    String year = url.substring(pos + 10, pos + 14);
    if (!StringParser::isNumeric(year)) {
        return String("");
    }
    if ((year.toInt() > 3000) || (year.toInt() < 2020)) {
        return String("");
    }

    String month = url.substring(pos + 15, pos + 17);
    if (!StringParser::isNumeric(month)) {
        return String("");
    }
    if ((month.toInt() > 12) || (month.toInt() < 1)) {
        return String("");
    }

    String day = url.substring(pos + 18, pos + 20);
    if (!StringParser::isNumeric(day)) {
        return String("");
    }
    if ((day.toInt() > 31) || (day.toInt() < 1)) {
        return String("");
    }

    return hours + String(":") + minutes + String(" ") + year + String("-") + month + String("-") + day;
}