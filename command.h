#include <string>
#include <cstring>
#include "LastSectionReaderWriter.hpp"
using namespace std;

std::string titleStr = R"(	
 _   _ ____  ____    ____            _       _   ____                    _      
| | | / ___|| __ )  / ___|  ___ _ __(_) __ _| | |  _ \  ___  _ __   __ _| | ___ 
| | | \___ \|  _ \  \___ \ / _ \ '__| |/ _` | | | | | |/ _ \| '_ \ / _` | |/ _ \
| |_| |___) | |_) |  ___) |  __/ |  | | (_| | | | |_| | (_) | | | | (_| | |  __/
 \___/|____/|____/  |____/ \___|_|  |_|\__,_|_| |____/ \___/|_| |_|\__, |_|\___|
                                                                   |___/        															   
)";
//const char* title=titleStr.c_str(); 

const char* help_str="USB Super Serial Dongle. version %d.\n\
read command:\n\
    read_help = get help.\n\
    read_tip = get password tip.\n\
    read_id = get id of dongle.\n\
    read_count = get count of blocks.\n\
    read_block = get block information.\n";
const char* help_str2="write command:\n\
    write_password C P T = when giving current password(C) is correct, change password(P) and tip(T).\n\
    write_block C I M T S = when giving current password(C) is correct, according to the index of the target block(I), change mode(M), MAX_TIME(T), and seed(S).\n";

const char* remark_str1="\
parts of write_XXX command are separated by spaces, and all these parameters must not contain spaces.\n\
when using the write_XXX command, the current password must be correct.\n";
const char* remark_str2="if the current password is entered incorrectly %d consecutive times, the dongle will be permanently locked.\n";

const char* wrong_password="wrong password.Current password error count:%d.\n";
const char* wrong_command="wrong command,please input help command for help.";
const char* command_ok="command was processed correctly."; 

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


