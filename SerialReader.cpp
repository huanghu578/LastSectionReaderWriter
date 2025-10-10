#include "SerialReader.h"
constexpr uint16_t VID = 0x1234;
constexpr uint16_t PID = 0x4321;
constexpr uint16_t RID = 0x0001;
constexpr std::chrono::milliseconds FRAME_INTERVAL(200);

USBSerial serial(true, VID, PID, RID);
std::string received_data;
volatile bool data_ready = false;
Thread serial_thread;
Timeout flipper;
void worker() {
    while(1){
        while (serial.readable() && !data_ready) {//有数据已经准备好时，不能读入//TODO:是否应该丢弃
            flipper.attach([](){
                //received_data += '\0';
                if (!received_data.empty()) {
                    data_ready = true;
                }
            },FRAME_INTERVAL);
            char c;
            if (serial.read(&c, 1) > 0) {
                /* if (c == '\n' || c == '\r') {
                    // 遇到结束符，标记数据就绪
                    //received_data += '\0';
                    if (!received_data.empty()) {
                        data_ready = true;
                    }
                } else {
                    // 将字符添加到字符串
                    received_data += c;
                } */
               if((c>=32) || (c<=126)){//TODO:只要可见字符
                   received_data += c;
               }
            }
        }
        ThisThread::sleep_for(5ms);
    }
}
void serial_init() {    
    serial_thread.start(&worker);
}