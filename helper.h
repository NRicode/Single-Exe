#pragma once
#include <string>

std::string generateRandomString(size_t length);
void replaceSubstring(std::string &str, const std::string &old_sub, const std::string &new_sub);
std::string removeFirstWord(const std::string str);
std::string getFirstWord(const std::string& str);
