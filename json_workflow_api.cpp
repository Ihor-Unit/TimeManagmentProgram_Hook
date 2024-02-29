#include "json_workflow_api.h"


namespace json {
    void AddStrings(std::wstring str1, std::wstring str2, GenericDocument<UTF16<>> &doc) {

        GenericValue<UTF16<>> val1(str1.c_str(), doc.GetAllocator());
        GenericValue<UTF16<>> val3(str2.c_str(), doc.GetAllocator());
        doc.AddMember(val1, val3, doc.GetAllocator());//USE ONLY Value TO PASS AS AN ARGUMENT ELSE IT'LL BRAKE
    }
}