#include <string>
#include <cstring>
#include "LastSectionReaderWriter.hpp"
#include "serial_str.h"
using namespace std; 

string read_help();
string read_tip();
uint32_t read_id();
uint32_t read_version();
uint8_t read_count();
bool read_block(uint8_t block_index,block_t &b);
bool read_response(uint8_t block_index,string query_code,uint64_t &response);
bool write_block(string old_password,uint8_t block_index,block_t b);
bool write_password(string old_password,string new_password,string new_tip);
bool write_init_setting();