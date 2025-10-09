#include "SerialReader.h"
constexpr uint16_t VID = 0x1234;
constexpr uint16_t PID = 0x4321;
constexpr uint16_t RID = 0x0001;
USBSerial serial(true, VID, PID, RID);
volatile std::string received_data;
volatile bool data_ready = false;
void serial_rx_isr() {
    while (serial.readable() && !data_ready) {//有数据已经准备好时，不能读入//TODO:是否应该丢弃
        char c;
        if (serial.read(&c, 1) > 0) {
            if (c == '\n' || c == '\r') {
                // 遇到结束符，标记数据就绪
                if (!received_data.empty()) {
                    data_ready = true;
                }
            } else {
                // 将字符添加到字符串
                received_data += c;
            }
        }
    }
}
void serial_init() {    
    serial.sigio(callback(serial_rx_isr));
}