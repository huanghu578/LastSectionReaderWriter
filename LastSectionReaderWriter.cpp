#include "LastSectionReaderWriter.hpp"
#include <cstring>
setting_t current_setting;
void PrintCurrentSetting() {
    printf("Password:%s \n", current_setting.password.data());    
    printf("Password Tip:%s \n", current_setting.passwordTip.data());    
    printf("Locked: %d\n", current_setting.locked);
    printf("ID: %d\n", current_setting.id);
    for (uint16_t i = 1; i <= TYPE; i++) {
        printf("Block %zu: Mode:%d, Max Try:%d, Current Try:%d, Block Seed:%s\n", i,
               current_setting.blocks[i-1].mode,
               current_setting.blocks[i-1].max_try,
               current_setting.blocks[i-1].current_try,
               "xxx");
    }
}

void InitCurrentSetting() {
    const char* src = INIT_PASSWORD;
    string srcs=string(src);    
    std::copy(src, src + srcs.length() + 1, current_setting.password.begin()); // +1,包括'\0'
    const char* src2 = INIT_TIP;
    string srcs2=string(src2);
    std::copy(src2, src2 + srcs2.length() + 1, current_setting.passwordTip.begin()); // +1,包括'\0'
    current_setting.locked = LOCKED255;
    current_setting.id = (uint32_t)time(NULL);
    current_setting.password_try_time=0;    
    for (uint8_t seed = 1; seed <= TYPE; seed++)
    {
        current_setting.blocks[seed-1] = {MODE255, INIT_MAX_TRY, 0, seed};
    }
}
#ifdef __MBED__
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
        #ifdef __MBED__
            TG_ERR_FILE_FUN_LINE();
        #endif        
    }
    return true;
}
LastSectionReaderWriter::LastSectionReaderWriter() {
    if (this->init() != 0) {
        #ifdef __MBED__
            TG_ERR_FILE_FUN_LINE();
        #endif
    }
    _flashStartAddress = this->get_flash_start();
    _flashSize = this->get_flash_size();
    _pageSize = this->get_page_size();
    _lastSectorSize =
        this->get_sector_size(this->_flashStartAddress + this->_flashSize - 1);
    if (sizeof(setting_t) > _lastSectorSize) {
        #ifdef __MBED__
            TG_ERR_FILE_FUN_LINE();
        #endif
    }
    _flashEndAddress = this->_flashStartAddress + this->_flashSize;
    _sectorSize = this->get_sector_size(this->_flashEndAddress - 1);
    _lastSectorStart = this->_flashEndAddress - this->_lastSectorSize;
    TG_DEBUG_FILE_FUN_LINE(ok);
}
void LastSectionReaderWriter::WriteCurrentSetting() {
    if (_eraseLastSector() == true) {
        if (this->program(&current_setting, this->_lastSectorStart,sizeof(setting_t)) != 0) {
            #ifdef __MBED__
                TG_ERR_FILE_FUN_LINE();
            #endif
        }
    }
    PrintCurrentSetting();    
    #ifdef __MBED__
        TG_DEBUG_FILE_FUN_LINE(ok);
    #endif    
}

void LastSectionReaderWriter::ReadCurrentSetting() {
    if (this->read(&current_setting, this->_lastSectorStart,sizeof(setting_t)) != 0) {
        #ifdef __MBED__
            TG_ERR_FILE_FUN_LINE();
        #endif
    }
    if (current_setting.id == AUTO_INIT_CURRENT_SETTING_ID) {//自动初始化
        InitCurrentSetting();
        WriteCurrentSetting();
    }
    #ifdef __MBED__
        TG_DEBUG_FILE_FUN_LINE(ok);
    #endif  
}
#endif