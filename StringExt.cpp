#include "StringExt.h" 
bool STRING_equal(const string& s1,const string& s2){//TODO:因为mbed的string没有==操作符    
    if(s1.size()!=s2.size()) return false;
    for(int i=0;i<s1.size();i++){
        if(s1[i]!=s2[i]) return false;
    }
    return true;
}
void _remove_invisible_chars(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) {        
        return (c < 32 ) || (c > 126); 
    }), str.end());
}
vector<string> split_by_space(const string& str) {    
    vector<string> tokens;
    stringstream ss(str);
    string tmp; 
    while (std::getline(ss, tmp, ' ')) {
        _remove_invisible_chars(tmp);
        string token = trim(tmp);
		if (!token.empty() && token!=string("\n")) { // 检查是否为空字符串
			tokens.push_back(token);
		}
	}
    return tokens;
}
string to_lower(string str) {
    string s;
    transform(str.begin(), str.end(), s.begin(), ::tolower);
    return s;
}
bool canConvertToInt(const std::string& str) {
    char* end;
    long num = std::strtol(str.c_str(), &end, 10);
    if (end == str.c_str() || *end != '\0' || errno == ERANGE) {
        return false;
    }
    return true;
}
string trim(const string& str) {
    auto begin = find_if_not(str.begin(), str.end(), [](unsigned char c) {
        return isspace(c);
    });
    auto end = find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
        return isspace(c);
    }).base();    
    return string(begin, end);
}
/* bool startsWith(const string& str, const string& prefix) {
    return str.find(prefix) == 0;
}
bool  IsContainsStr(string str,string contains_str){
	string::size_type idx = str.find(contains_str);
	if (idx!=string::npos)	{
		return true;
	}
	else	{
		return false;
	}
} */