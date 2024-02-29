#pragma once
#include <string>
#include <ctime>

const std::string CurrentDateTime();

std::wstring ToCustomTime(time_t sec);

time_t FromCustomTime(std::wstring time);