#include "main.h"
#ifdef __MBED__
USBSerial serial(true, VID, PID, RID);
DigitalOut led(LED1);
#endif
void WrongCommand(string s);
void OkCommand(string s);
int main() {
#ifdef ENABLE_FLASH_PROTECT
    ReadoutProtection(ENABLE);
    if (SetLevel1IfLevel0()) {
        LedLongOn(led);
    }
#endif
    read_current_setting();
    while (1) {
        uint8_t ptr[783];
        uint32_t readLen;
        serial.receive_nb((ptr), sizeof(ptr), &readLen);
        std::string serialRead(reinterpret_cast<const char*>(&ptr[0]), readLen);
        if (readLen > 0) {
            // read command
            if (to_lower(trim(serialRead)) == "read_help") {
                serial.printf("%s\n", read_help().c_str());
                goto loop;
            }
            if (to_lower(trim(serialRead)) == "read_tip" &&
                current_setting.locked == LOCKED255) {
                serial.printf("%s\n", current_setting.passwordTip.data());
                goto loop;
            }
            if (to_lower(trim(serialRead)) == "read_id") {
                serial.printf("%d\n", current_setting.id);
                goto loop;
            }
            if (to_lower(trim(serialRead)) == "read_version") {
                serial.printf("%d\n", VERSION);
                goto loop;
            }
            if (to_lower(trim(serialRead)) == "read_count" &&
                current_setting.locked == LOCKED255) {
                serial.printf("%d\n", TYPE);
                goto loop;
            }
            if (to_lower(trim(serialRead)) == "read_block" &&
                current_setting.locked == LOCKED255) {
                vector<string> parameters =
                    split(serialRead, " ");  // 命令、index
                if (parameters.size() == 2) {
                    string block_info;
                    if (read_block(parameters[1], block_info)) {
                        serial.printf("%s\n", block_info.c_str());
                        goto loop;
                    }
                }
            }
            if (to_lower(trim(serialRead)) == "read_response" &&
                current_setting.locked == LOCKED255) {
                vector<string> parameters =
                    split(serialRead, " ");  // 命令、index、query_code
                if (parameters.size() == 3) {
                    uint64_t response;
                    if (read_response(parameters[1], parameters[2], response)) {
                        serial.printf("%llu\n", response);
                        goto loop;
                    }
                }
                // WrongCommand("command
                // error!");//因为此命令是close的，所以错误也不应该反馈
                goto loop;
            }
            // write command
            if (to_lower(trim(serialRead)) == "write_dongle" &&
                current_setting.locked == LOCKED255) {
                vector<string> parameters = split(serialRead, " ");
                if (parameters.size() == 4 &&
                    write_dongle(parameters[1], parameters[2], parameters[3])) {
                    OkCommand("write_password command OK!");
                    current_setting.password_try_time =
                        0;  // TODO：为减少写flash次数，密码正确时不写flash
                    goto loop;
                }
                if (parameters[1] != string(current_setting.password.data())) {
                    // 密码错误,可能有人爆破
                    current_setting.password_try_time += 1;
                    write_init_setting();
                    serial.printf(wrong_password,
                                  current_setting.password_try_time);
                    goto loop;
                }
            }
            if (to_lower(trim(serialRead)) ==
                "write_init_setting"  // && current_setting.locked == LOCKED255
            ) {  // 此处留下后门，在锁住也可以复位
                write_init_setting();
                // OkCommand("write_init_setting command
                // OK!");//因为此命令是close的，所以不应该反馈
                goto loop;
            }
            if (to_lower(trim(serialRead)) == "write_block" &&
                current_setting.locked == LOCKED255) {
                vector<string> parameters = split(serialRead, " ");
                if (parameters.size() ==
                    6) {  // 命令、密码、index、mode、max_try、seed
                    if (write_block(parameters[1], parameters[2], parameters[3],
                                    parameters[4], parameters[5])) {
                        current_setting.password_try_time =
                            0;  // TODO：为减少写flash次数，密码正确时不写flash
                        OkCommand("write_block command OK!");
                        goto loop;
                    }
                    if (parameters[1] !=
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
void WrongCommand(string s) {
    serial.printf("%s ", wrong_command);
    serial.printf("%s\n", s.c_str());
}
void OkCommand(string s) {
    serial.printf("%s ", command_ok);
    serial.printf("%s\n", s.c_str());
}