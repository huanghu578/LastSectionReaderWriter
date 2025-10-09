#include "serial_str.h"
string AsciiArtStr = R"(	
 _   _ ____  ____    ____            _       _   ____                    _      
| | | / ___|| __ )  / ___|  ___ _ __(_) __ _| | |  _ \  ___  _ __   __ _| | ___ 
| | | \___ \|  _ \  \___ \ / _ \ '__| |/ _` | | | | | |/ _ \| '_ \ / _` | |/ _ \
| |_| |___) | |_) |  ___) |  __/ |  | | (_| | | | |_| | (_) | | | | (_| | |  __/
 \___/|____/|____/  |____/ \___|_|  |_|\__,_|_| |____/ \___/|_| |_|\__, |_|\___|
                                                                   |___/        															   
)";

const string help_str = R"(USB Super Serial Dongle.command:
help = get help.
tip = get password tip.
id = get id of dongle.
blocks = get count of blocks.
block index = get information of block index.
query index query_string = get hash value from block no. index according query_string.
write_dongle password new_password new_tip = write new_password & new_tip to dongle by checking password. 
write_block password index mode max_time seed = write mode,max_try,seed to block no. index by checking password.
parts of these commands are separated by spaces, and all these parameters must not contain spaces.)";

const string wrong_command =R"(wrong command,please input help for more information. 
Please check:
a.index must be positive integer and  within the range.
b.password,new_password,new_tip must less than 256 bytes.
c.mode must be 1 or 0. 1 means lifetime license mode, and 0 means pay-per-use mode.
d.max_time must be 16 bit positive integer. The maximum number of trials for the current block, when mode 0.
e.seed must be 32 bit positive integer.)";

const char* wrong_password = "current password error count:%d.\n";