#include "main.h"
DigitalOut led(LED1);
int main() {
#ifdef ENABLE_FLASH_PROTECT
    ReadoutProtection(ENABLE);
    if (SetLevel1IfLevel0()) {
        LedLongOn(led);
    }
#endif
    read_current_setting();
    serial_init();
    while (1) {
        if (data_ready) {
            vector<string> parts = split_by_space(received_data);
            
            auto command = to_lower((parts[0]));
            for(auto c:parts){
                serial.printf("%s,", c.c_str());
            }
            serial.printf("command:%s,parts[0].size:%d,parts.size:%d\n",parts[0].c_str(),parts[0].size(),parts.size());
            if (parts.size() == 1) {  // 单命令
                MBED_DEBUG_FILE_FUN_LINE(单命令);
                if (command.find("help")!=string::npos) {
                    CriticalSectionLock::enable();
                    serial.printf("%s\n", AsciiArtStr.c_str());
                    serial.printf("%s\n", help_str.c_str());
                    CriticalSectionLock::disable();
                    goto loop;
                }
                if (command == string("tip")) {
                    MBED_DEBUG_FILE_FUN_LINE(tip);
                    serial.printf("%s\n", current_setting.passwordTip.data());
                    goto loop;
                }
                if (STRING_equal(command,string("id"))) {
                    serial.printf("%d\n", current_setting.id);
                    goto loop;
                }
                if (command == "ver") {
                    serial.printf("%d\n", VERSION);
                    goto loop;
                }
                if (command == "blocks") {
                    serial.printf("%d\n", TYPE);
                    goto loop;
                }
            }
            if (parts.size() == 1 &&
                (command ==
                 "write_init_setting")) {  // 此命令不能简写，防止用户猜到
                write_init_setting();
                goto loop;
            }
            if (parts.size() == 2 && (command == "block")) {
                string block_info;
                if (block(parts[1], block_info)) {
                    serial.printf("%s\n", block_info.c_str());
                    goto loop;
                }
            }
            if (parts.size() == 3 && (command == "query")) {
                // 命令、index、query_code
                uint64_t response;
                if (query(parts[1], parts[2], response)) {
                    serial.printf("%llu\n", response);
                    goto loop;
                }
            }
            // write command
            if (parts.size() == 4 && command == "write_dongle") {
                if (write_dongle(parts[1], parts[2], parts[3])) {
                    OkCommand(command);                    
                    goto loop;
                }
                if (parts[1] != string(current_setting.password.data())) {
                    // 密码错误,可能有人爆破
                    current_time += 1;
                    serial.printf(wrong_password, current_time);
                    goto loop;
                }
            }
            if (parts.size() == 6 && command == "write_block") {
                // 命令、密码、index、mode、max_try、seed
                if (write_block(parts[1], parts[2], parts[3], parts[4],
                                parts[5])) {                    
                    OkCommand(command);
                    goto loop;
                }
                if (parts[1] != string(current_setting.password.data())) {
                    // 密码错误,可能有人爆破
                    current_time += 1;
                    serial.printf(wrong_password, current_time);
                    goto loop;
                }
            }
            WrongCommand();
        }
    loop:
        data_ready = false;
        received_data.clear();
        led = !led;
        delay_for_defense();
    }
}
void WrongCommand() {
    CriticalSectionLock::enable();
    serial.printf("%s\n", wrong_command.c_str());
    CriticalSectionLock::disable();
}
void OkCommand(string command) {
    current_time = 0;
    serial.printf("%s\n", (command + string(" OK!")).c_str());
}