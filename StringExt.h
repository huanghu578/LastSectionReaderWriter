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

string trim(const string& str);
string to_lower(string str);
bool canConvertToInt(const string& str);
vector<string> split_by_space(const string& str);
bool STRING_equal(const string& s1,const string& s2);

/* bool  IsContainsStr(string str,string contains_str);
bool startsWith(const string& str, const string& prefix); */

