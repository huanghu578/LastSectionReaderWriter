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
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm> // 用于 std::remove_if 
using std::string;
using std::vector;
using std::stringstream;
void remove_invisible_chars(std::string& str);
string trim(const string& str);
bool startsWith(const string& str, const string& prefix);
string to_lower(string str);
bool canConvertToInt(const string& str);
bool  IsContainsStr(string str,string contains_str);

//vector<string> Vs2Vs(const vector<string> vs1);
//string uint8ArrayToHexString(const uint8_t arr[], size_t length);
vector<string> split_by_space(const string& str);