#pragma once
#include <cstring>
#include <string>
#include "LastSectionReaderWriter.hpp"
#include "StringExt.h"
#include "serial_str.h"
#include <string>
using std::string;
using namespace std; 
//单命令直接在main里处理
//所有的string参数在传入前去除不可见前后的字符及空白
//所有的判断（参数合法性、密码正确性、是否锁定等）都在此完成，所以才传递string类型的参数
bool read_block(string index,string &block_info);
bool read_response(string index, string query_code, uint64_t& response);
bool write_dongle(string current_password,string new_password,string new_tip);
bool write_block(string current_password, string index, string mode, string max_try, string seed);
bool write_init_setting();

