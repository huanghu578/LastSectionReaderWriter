#include "LastSectionReaderWriter.hpp"
#include <cstring>

setting_t current_setting;
void PrintCurrentSetting() {
    printf("Password:%s \n", current_setting.password.data());    
    printf("Password Tip:%s \n", current_setting.passwordTip.data()); 
    printf("ID: %d\n", current_setting.id);
    for (uint16_t i = 1; i <= TYPE; i++) {
        auto mode=current_setting.blocks[i - 1].mode ? "1(lifetime license mode)" : "0(pay-per-use mode)";
        printf("Block %zu: Mode:%s, Max Try:%d, Current Try:%d, Block Seed:xxx\n", 
                i,
                mode,
                current_setting.blocks[i-1].max_try,
                current_setting.blocks[i-1].current_try);
    }
}

void InitCurrentSetting() {
    std::copy(INIT_PASSWORD, INIT_PASSWORD + sizeof(INIT_PASSWORD), current_setting.password.begin()); // +1,包括'\0'
    std::copy(INIT_TIP, INIT_TIP + sizeof(INIT_TIP), current_setting.passwordTip.begin()); // +1,包括'\0'
    current_setting.id =djb2(to_string((uint32_t)time(NULL)));
    for (uint8_t seed = 1; seed <= TYPE; seed++)
    {
        current_setting.blocks[seed-1] = {MODE255, INIT_MAX_TRY, 0, seed};
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
