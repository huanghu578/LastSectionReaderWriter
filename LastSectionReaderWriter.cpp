#include "LastSectionReaderWriter.hpp"
#include <cstring>
const string INIT_PASSWORD = "197001010000000";
const string INIT_TIP = "TIME_STAMP_OF_UNIX";
constexpr uint8_t MAX_BYTES_TO_PASSWORD = 255;
constexpr int AUTO_INIT_CURRENT_SETTING_ID = 0xFFFFFFFF;
constexpr int VERSION = 202608;
constexpr uint8_t TYPE = 40;  // 类型,不同档次产品的区分
constexpr uint16_t INIT_MAX_TRY = 20;
constexpr uint8_t MODE_PAY_PER_USE = 0;    // 次数模式
constexpr uint8_t MODE_LIFTIME = 1;  // 正常模式
setting_t current_setting;
void PrintCurrentSetting() {
    printf("Password:%s, ", current_setting.password.c_str());    
    printf("Password Tip:%s, ", current_setting.passwordTip.c_str()); 
    printf("ID: %d\n", current_setting.id);
    for (uint16_t i = 1; i <= TYPE; i++) {
        auto mode = current_setting.blocks[idx - 1].mode
                        ? string("1(lifetime license mode)")
                        : string("0(pay-per-use mode)");
        printf("Block %zu: Mode:%s, Max Try:%d, Current Try:%d, Block Seed:xxx\n", 
                i,
                mode.c_str(),
                current_setting.blocks[i-1].max_try,
                current_setting.blocks[i-1].current_try);
    }
}
void InitCurrentSetting() {
    current_setting.password = INIT_PASSWORD;
    current_setting.passwordTip = INIT_TIP;
    current_setting.id =djb2(to_string((uint32_t)time(NULL)));
    for (uint8_t index = 1; index <= TYPE; index++) {
        current_setting.blocks[index - 1] = {MODE_LIFTIME, INIT_MAX_TRY, 0,
                                             index};
    }
}
void write_current_setting(){
    auto f = new LastSectionReaderWriter();
    f->WriteCurrentSetting();
    delete f;
}
void read_current_setting() {
    auto f = new LastSectionReaderWriter();
    f->ReadCurrentSetting();
    delete f;
}
bool LastSectionReaderWriter::_eraseLastSector() {
    if (this->erase(this->_lastSectorStart, this->_sectorSize) != 0) {
        MBED_ERR_FILE_FUN_LINE();
    }
    return true;
}
LastSectionReaderWriter::LastSectionReaderWriter() {
    if (this->init() != 0) {
        MBED_ERR_FILE_FUN_LINE();
    }
    _flashStartAddress = this->get_flash_start();
    _flashSize = this->get_flash_size();
    _pageSize = this->get_page_size();
    _lastSectorSize =
        this->get_sector_size(this->_flashStartAddress + this->_flashSize - 1);
    if (sizeof(setting_t) > _lastSectorSize) {
        MBED_ERR_FILE_FUN_LINE();
    }
    _flashEndAddress = this->_flashStartAddress + this->_flashSize;
    _sectorSize = this->get_sector_size(this->_flashEndAddress - 1);
    _lastSectorStart = this->_flashEndAddress - this->_lastSectorSize;
    MBED_DEBUG_FILE_FUN_LINE(ok); 
}
void LastSectionReaderWriter::WriteCurrentSetting() {
    if (_eraseLastSector() == true) {
        if (this->program(&current_setting, this->_lastSectorStart,sizeof(setting_t)) != 0) {
            MBED_ERR_FILE_FUN_LINE();
        }
    }
    PrintCurrentSetting();    
    MBED_DEBUG_FILE_FUN_LINE(ok);    
}

void LastSectionReaderWriter::ReadCurrentSetting() {
    if (this->read(&current_setting, this->_lastSectorStart,sizeof(setting_t)) != 0) {
        MBED_ERR_FILE_FUN_LINE();
    }
    if (current_setting.id == AUTO_INIT_CURRENT_SETTING_ID) {//自动初始化
        InitCurrentSetting();
        WriteCurrentSetting();
    }    
    MBED_DEBUG_FILE_FUN_LINE(ok);      
}
