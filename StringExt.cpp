#include "StringExt.h" 
void remove_invisible_chars(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) {        
        return (c < 32 ) || (c > 126); 
    }), str.end());
}

vector<string> split_by_space(const string& str) {    
    vector<string> tokens;
    stringstream ss(str);
    string tmp; 
    while (std::getline(ss, tmp, ' ')) {
        remove_invisible_chars(tmp);
        string token = trim(tmp);
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



