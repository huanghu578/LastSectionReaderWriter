#include "main.h"
USBSerial serial(true, VID, PID, RID);
DigitalOut led(LED1);
uint8_t ptr[800]={0};//uint8_t ptr[800];//783=256*3+13
uint32_t readLen;
int main() {
#ifdef ENABLE_FLASH_PROTECT
    ReadoutProtection(ENABLE);
    if (SetLevel1IfLevel0()) {
        LedLongOn(led);
    }
#endif
    read_current_setting(); 
    serial.attach([](){
        serial.receive_nb((ptr), sizeof(ptr), &readLen);
    });
    while (1) {        
        if (readLen > 0) {             
            std::string serialRead=string((char*)ptr);
            serialRead[readLen]='\0';
            vector<string> parts = split_by_space(serialRead);
            //vector<string> parts=Vs2Vs(parts1);             
            auto c1 =to_lower((parts[0]));
            auto c=c1.c_str();
            auto command=string(c);
            serial.printf("%s\n",command.c_str());
            for(auto c:parts){
                serial.printf("%s,",c.c_str());
            }
            serial.printf("\n");
            if (parts.size() == 1){ //单命令                
                if (command == "read_help") {
                    serial.printf("%s\n", read_help().c_str());
                    goto loop;
                }                
                if ((command == "read_tip") && 
                    current_setting.locked == LOCKED255) {
                    serial.printf("%s\n", current_setting.passwordTip.data());
                    goto loop;
                }
                if (command=="read_id") {
                    serial.printf("%d\n", current_setting.id);
                    goto loop;
                }
                if (command == "read_ver") {
                    serial.printf("%d\n", VERSION);
                    goto loop;
                }
                if (command == "read_count" &&
                    current_setting.locked == LOCKED255) {
                    serial.printf("%d\n", TYPE);
                    goto loop;
                }
                if (command == "write_init_setting") {
                    write_init_setting();
                    goto loop;
                } 
            }

            if (command == "read_block" &&
                current_setting.locked == LOCKED255) {
                if (parts.size() == 2) {
                    string block_info;
                    if (read_block(parts[1], block_info)) {
                        serial.printf("%s\n", block_info.c_str());
                        goto loop;
                    }
                }
            }
            if (command == "read_response" &&
                current_setting.locked == LOCKED255) {
                // 命令、index、query_code
                if (parts.size() == 3) {
                    uint64_t response;
                    if (read_response(parts[1], parts[2], response)) {
                        serial.printf("%llu\n", response);
                        goto loop;
                    }else {
                        serial.printf("read_response false\n");
                    }
                }                
            }
            // write command
            if (command == "write_dongle" &&
                current_setting.locked == LOCKED255) {
                if (parts.size() == 4 &&
                    write_dongle(parts[1], parts[2], parts[3])) {
                    OkCommand("write_password command OK!");
                    current_time=0;
                    goto loop;
                }
                if (parts[1] != string(current_setting.password.data())) {
                    // 密码错误,可能有人爆破
                    current_time += 1;
                    //write_init_setting();
                    serial.printf(wrong_password,current_time);
                    goto loop;
                }
            }

            if (command == "write_block" &&
                current_setting.locked == LOCKED255) {
                // 命令、密码、index、mode、max_try、seed
                if (parts.size() == 6) {  
                    if (write_block(parts[1], parts[2], parts[3],
                                    parts[4], parts[5])) {                        
                        current_time=0;  
                        OkCommand("write_block command OK!");
                        goto loop;
                    }
                    if (parts[1] !=string(current_setting.password.data())) {
                        // 密码错误,可能有人爆破
                        current_time += 1;
                        //write_init_setting();
                        serial.printf(wrong_password,current_time);
                        goto loop;
                    }
                }
            }

            WrongCommand("command or parameter err!");
            goto loop;
        }
    loop:
        readLen=0;
        memset(ptr, 0, sizeof(ptr)); // 将所有元素设置为0
        /* if (current_setting.password_try_time == MAX_PASSWORD_TIME) {
            current_setting.locked = LOCKED0;
            write_init_setting();
        } */
        led = !led;
        //ThisThread::sleep_for(2s);  // 人为增加密码尝试间隔,以增加爆破的难度
        delay_for_defense();
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