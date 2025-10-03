#include "StringExt.h" 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm> // 用于 std::remove_if 
std::vector<std::string> split_by_space(const string& str) {    
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string tmp; 
    while (std::getline(ss, tmp, ' ')) {
        auto token=remove_invisible_chars(trim(tmp));
		if (!token.empty() && token!=string("\n")) { // 检查是否为空字符串
			tokens.push_back(token);
		}
	}
    return tokens;
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
bool startsWith(const string& str, const string& prefix) {
    return str.find(prefix) == 0;
}
/*
vector<string> split(string str, string pattern){
    vector<string> ret;
    if(pattern.empty()) return ret;
    size_t start=0,index=str.find_first_of(pattern,0);
    while(index!=str.npos)    {
        if(start!=index) ret.push_back(str.substr(start,index-start));
        start=index+1;
        index=str.find_first_of(pattern,start);
    }
    if(!str.substr(start).empty()) ret.push_back(str.substr(start));
    return ret;
}*/

string to_lower(string str){
    string s;
    transform(str.begin(),str.end(),s.begin(),::tolower); 
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

//判断字符串是否包含某字符串
//@params:str:大的字符串
//@params:contains_str:所包含的字符串
//@return: false:未包含 true：包含
bool  IsContainsStr(string str,string contains_str){
	string::size_type idx = str.find(contains_str);
	if (idx!=string::npos)	{
		return true;
	}
	else	{
		return false;
	}
}

std::string remove_invisible_chars(std::string str) {    
    std::string::iterator new_end = std::remove_if(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c); // 删除所有空白字符
    });
    str.erase(new_end, str.end()); // 擦除从new_end到末尾的部分    
    return str;
}

vector<string> Vs2Vs(const vector<string> vs1){
    //vs1.erase(std::remove_if(vs1.begin(), vs1.end(), shouldRemove), vs1.end());//TODO:hh
    vector<string> vs2;
    /*
    for(uint8_t i=0;i<vs1.size();i++){
        const char *c=((vs1[i])).c_str();//const char *c=(remove_invisible_chars(vs1[i])).c_str();
        vs2.push_back(string(c));
    }*/
    for(const auto str:vs1){
        auto c=remove_invisible_chars(str);
        if (c!=""){
            vs2.push_back(string(c));
        }        
    }
    return vs2;
}