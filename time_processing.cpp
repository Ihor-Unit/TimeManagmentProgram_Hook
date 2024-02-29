#include "time_processing.h"

const std::string CurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct {};
    char       buf[20];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%m-%d-%Y", &tstruct);

    wchar_t wdest[20];
    mbstowcs(wdest, buf, 20);

    return buf;
}

std::wstring ToCustomTime(time_t sec) {
    time_t h = 0, m = 0, s = 0;
    if (sec >= 60 * 60) {
        h = sec / 3600;
        sec -= h * 3600;
    }
    if (sec >= 60) {
        m = sec / 60;
        sec -= m * 60;
    }
    s = sec;
    return std::to_wstring(h) + L"h " + std::to_wstring(m) + L"m " + std::to_wstring(s) + L"s\0";
}

time_t FromCustomTime(std::wstring time) {
    return std::stoi(time.substr(0, time.find('h'))) * 3600 +
        std::stoi(time.substr(time.find('h') + 2, time.find('m'))) * 60 +
        std::stoi(time.substr(time.find('m') + 2, time.find('s')));
}