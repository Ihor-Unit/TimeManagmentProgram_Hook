#pragma once
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/encodings.h"

namespace json {
	using namespace rapidjson;

	void AddStrings(std::wstring str1, std::wstring str2, GenericDocument<UTF16<>>& doc);
}