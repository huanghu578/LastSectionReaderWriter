#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <vector>
#include <string>
#include <cstdlib> // for strtol
#include <cerrno>  // for errno 
using std::string;
using std::vector;
string trim(const string& str);
bool startsWith(const string& str, const string& prefix);
vector<string> split(string str, string pattern);
string to_lower(string str);
bool canConvertToInt(const std::string& str);