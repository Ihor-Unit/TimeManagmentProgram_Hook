#include <windows.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include "rapidjson/document.h"

#include "json_stream_api.h"
#include "json_workflow_api.h"
#include "time_processing.h"

using namespace rapidjson;

bool cmp(const std::pair<std::wstring, size_t> &a, const std::pair<std::wstring, size_t>& b) {
    return a.second > b.second;
}

std::vector<std::pair<std::wstring, size_t>> mapSort(const std::map<std::wstring, size_t> &map) {
    std::vector<std::pair<std::wstring, size_t>> vect;
    
    for (auto& it : map) {
        vect.push_back(it);
    }

    std::sort(vect.begin(), vect.end(), cmp);
    return vect;
}

std::wstring GetActiveWindowTitle() {
    wchar_t* wndTitle = nullptr;
    std::wstring str(L"");

    HWND hwnd = GetForegroundWindow();
    if (hwnd != NULL) {
        int textSize = GetWindowTextLengthW(hwnd) + 1;

        wndTitle = new wchar_t[textSize];

        GetWindowTextW(hwnd, wndTitle, textSize);

        str = wndTitle;

        delete[] wndTitle;
    }
    return str;
}

void WriteDataToFile(std::string filename, const std::map<std::wstring, size_t> &chunk) {
    GenericDocument<UTF16<>> doc;
    size_t time_total = 0;

    std::vector<std::pair<std::wstring, size_t>> sortedMap;

    //you can not sort a map by value. They are internally sorted only by keys 
    sortedMap = mapSort(chunk);

    doc.Parse(L"{}");//prepare the document
    for (auto iterat = sortedMap.begin(); iterat != sortedMap.end(); ++iterat) {
        time_total += iterat->second;
        
        json::AddStrings(iterat->first, ToCustomTime(iterat->second), doc);
    }
    json::AddStrings(L"Time total", ToCustomTime(time_total), doc);

    json::WriteInUTF(doc, filename.c_str());
}

void ReadExistingData(const std::string const &filename, std::map<std::wstring, size_t>&chunk) {
    GenericDocument<UTF16<>> doc;

    if (json::ReadFromUTF(doc, filename.c_str()) == 0) {

        for (auto iter = doc.MemberBegin(); iter != doc.MemberEnd(); iter++) {
            if (wcscmp(iter->name.GetString(), L"Time total") == 0 ) {
                continue;
            }
            chunk[iter->name.GetString()] = FromCustomTime(iter->value.GetString());
        }

    }
}


int main(){
#ifndef _DEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // _DEBUG
    //ShowWindow(GetConsoleWindow(), SW_HIDE);
    while (1) {

        std::string today = CurrentDateTime();
        const std::string file = today + ".json";

        std::map<std::wstring, size_t> data;

        ReadExistingData(file, data);

        time_t t_bef = time(0), t_after;
        for (std::wstring prev = GetActiveWindowTitle();;) {
            if (today != CurrentDateTime()) break;
            

            if (prev == L"")
                prev = L"*No Caption Detected*";

            std::wstring curr = GetActiveWindowTitle();
            if (curr == L"")
                curr = L"*No Caption Detected*";


            if (curr.compare(prev) != 0) {//switched a programm
                t_after = time(0);
                time_t t_passed = (t_after - t_bef);


                if (data[prev] == 0) {
                    data[prev] = t_passed;
                    WriteDataToFile(file, data);

                    t_bef = time(0);
                }
                else {
                    time_t temp_datTime = data[prev];
                    data[prev] = static_cast<size_t>(temp_datTime + t_passed);
                    WriteDataToFile(file, data);

                    t_bef = time(0);
                }

                prev = curr;
            }

            Sleep(50);
        }
    }
    return 0;
} 