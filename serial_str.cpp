#include "serial_str.h"
string titleStr = R"(	
 _   _ ____  ____    ____            _       _   ____                    _      
| | | / ___|| __ )  / ___|  ___ _ __(_) __ _| | |  _ \  ___  _ __   __ _| | ___ 
| | | \___ \|  _ \  \___ \ / _ \ '__| |/ _` | | | | | |/ _ \| '_ \ / _` | |/ _ \
| |_| |___) | |_) |  ___) |  __/ |  | | (_| | | | |_| | (_) | | | | (_| | |  __/
 \___/|____/|____/  |____/ \___|_|  |_|\__,_|_| |____/ \___/|_| |_|\__, |_|\___|
                                                                   |___/        															   
)";

const char* help_str =
    "USB Super Serial Dongle.\n\
read command:\n\
    read_help = get help.\n\
    read_tip = get password tip.\n\
    read_id = get id of dongle.\n\
    read_count = get count of blocks.\n\
    read_block X = get information of block X.\n";
const char* help_str2="write command:\n\
    write_password C P T = when giving current password(C) is correct, change password(P) and tip(T).\n\
    write_block C X M T S = when giving current password(C) is correct, according to the index of the target block(X), change mode(M), MAX_TIME(T), and seed(S).\n";

const char* remark_str="\
parts of these commands are separated by spaces, and all these parameters must not contain spaces.\n\
when using the write_XXX command, the current password must be correct.\n";


const char* wrong_password="current password error count:%d.\n";
const char* wrong_command="wrong command,please input read_help for help. Perhaps it is due to the following reasons:\n\
1. Password and tip exceed 256 bytes; \n\
2. The number or form of parameters does not meet the requirements; \n\
3. The mode can only be 0 or 1; \n\
4. The block index is not in numerical form or out of range; \n\
5. When writing about dongle, the information before and after is the same; \n\
6. When writing blocks, the information before and after is the same.\n";
const char* command_ok="command success.";

