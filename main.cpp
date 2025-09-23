#include "main.h"
USBSerial serial(true, VID, PID, RID);
DigitalOut led(LED1);

int main() {
#ifdef ENABLE_FLASH_PROTECT
    ReadoutProtection(ENABLE);
    if (SetLevel1IfLevel0()) {
        LedLongOn(led);
    }
#endif
    read_current_setting();
    serial.sync();
    while (1) {
        uint8_t ptr[800];//783=256*3+13
        uint32_t readLen;
        serial.receive_nb((ptr), sizeof(ptr), &readLen);
        std::string serialRead(reinterpret_cast<const char*>(&ptr[0]), readLen);
        vector<string> parts = split(serialRead, " ");
        if (readLen > 0) {
            if (parts.size() == 1){ //单命令
                if (to_lower(trim(parts[0])) == "read_help") {
                    serial.printf("%s\n", read_help().c_str());
                    goto loop;
                }
                if (to_lower(trim(parts[0])) == "read_tip" &&
                    current_setting.locked == LOCKED255) {
                    serial.printf("%s\n", current_setting.passwordTip.data());
                    goto loop;
                }
                if (to_lower(trim(parts[0])) == "read_id") {
                    serial.printf("%d\n", current_setting.id);
                    goto loop;
                }
                if (to_lower(trim(parts[0])) == "read_version") {
                    serial.printf("%d\n", VERSION);
                    goto loop;
                }
                if (to_lower(trim(parts[0])) == "read_count" &&
                    current_setting.locked == LOCKED255) {
                    serial.printf("%d\n", TYPE);
                    goto loop;
                }
                if (to_lower(trim(parts[0])) == "write_init_setting") {  
                    // TODO:此处留下后门，在锁住也可以复位// LOCKED255 // & // & // current_setting.locked ==
                    write_init_setting();
                    // OkCommand("write_init_setting command
                    // OK!");//因为此命令是close的，所以不应该反馈
                    goto loop;
                }
            }

            if (to_lower(trim(parts[0])) == "read_block" &&
                current_setting.locked == LOCKED255) {                
                if (parts.size() == 2) {
                    string block_info;
                    if (read_block(parts[1], block_info)) {
                        serial.printf("%s\n", block_info.c_str());
                        goto loop;
                    }
                }
            }
            if (to_lower(trim(parts[0])) == "read_response" &&
                current_setting.locked == LOCKED255) {
                // 命令、index、query_code
                if (parts.size() == 3) {
                    uint64_t response;
                    if (read_response(parts[1], parts[2], response)) {
                        serial.printf("%llu\n", response);
                        goto loop;
                    }
                }                
            }
            // write command
            if (to_lower(trim(parts[0])) == "write_dongle" &&
                current_setting.locked == LOCKED255) {                
                if (parts.size() == 4 &&
                    write_dongle(parts[1], parts[2], parts[3])) {
                    OkCommand("write_password command OK!");
                    // TODO：为减少写flash次数，密码正确时不写flash
                    current_setting.password_try_time =0;  
                    goto loop;
                }
                if (parts[1] != string(current_setting.password.data())) {
                    // 密码错误,可能有人爆破
                    current_setting.password_try_time += 1;
                    write_init_setting();
                    serial.printf(wrong_password,
                                  current_setting.password_try_time);
                    goto loop;
                }
            }

            if (to_lower(trim(parts[0])) == "write_block" &&
                current_setting.locked == LOCKED255) {                
                // 命令、密码、index、mode、max_try、seed
                if (parts.size() == 6) {  
                    if (write_block(parts[1], parts[2], parts[3],
                                    parts[4], parts[5])) {
                        // TODO：为减少写flash次数，密码正确时不写flash
                        current_setting.password_try_time =0;  
                        OkCommand("write_block command OK!");
                        goto loop;
                    }
                    if (parts[1] !=
                        string(current_setting.password.data())) {
                        // 密码错误,可能有人爆破
                        current_setting.password_try_time += 1;
                        write_init_setting();
                        serial.printf(wrong_password,
                                      current_setting.password_try_time);
                        goto loop;
                    }
                }
            }
            WrongCommand("command or parameter err!");
            goto loop;
        }
    loop:
        if (current_setting.password_try_time == MAX_PASSWORD_TIME) {
            current_setting.locked = LOCKED0;
            write_init_setting();
        }
        led = !led;
        ThisThread::sleep_for(2s);  // 人为增加密码尝试间隔,以增加爆破的难度
    }
}
