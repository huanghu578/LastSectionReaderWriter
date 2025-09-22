#include "StringExt.h" 
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