#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <vector>
#include <string>
#include <cstdlib> // for strtol
#include <cerrno>  // for errno 
#include <algorithm>
#include <string>
#include <iostream>
using std::string;
using std::vector;
string trim(const string& str);
bool startsWith(const string& str, const string& prefix);
string to_lower(string str);
bool canConvertToInt(const std::string& str);
bool  IsContainsStr(string str,string contains_str);
std::string remove_invisible_chars(std::string str);
vector<string> Vs2Vs(const vector<string> vs1);
std::string uint8ArrayToHexString(const uint8_t arr[], size_t length);
std::vector<std::string> split_by_space(const string& str) ;