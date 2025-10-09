#include <string>
#include "USBSerial.h"
#include "mbed.h"
extern USBSerial serial;
extern volatile std::string received_data;
extern volatile bool data_ready;
void serial_init();