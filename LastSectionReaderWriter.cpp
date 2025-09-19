#include "LastSectionReaderWriter.hpp"
#include "TgMbedHelper.h"
setting_t current_setting;

void PrintCurrentSetting() {
    printf("Password: ");
    for (auto c : current_setting.password) {
        printf("%c", c);
    }
    printf("\n");
    printf("Password Tip: ");
    for (auto c : current_setting.passwordTip) {
        printf("%c", c);
    }
    printf("\n");
    printf("Locked: %d\n", current_setting.locked);//TODO
}

void ResetCurrentSetting(){
    current_setting = {INIT_PASSWORD, INIT_TIP, NOT_LOCKED, NOT_ENABLE_TRY,
                       INIT_TRY_TIME};
}

bool LastSectionReaderWriter::_eraseLastSector() {
    if (this->erase(this->_lastSectorStart, this->_sectorSize) != 0) {
        TG_ERR_FILE_FUN_LINE();
    }
    return true;
}
LastSectionReaderWriter::LastSectionReaderWriter() {
    if (this->init() != 0) {
        TG_ERR_FILE_FUN_LINE();
    }
    _flashStartAddress = this->get_flash_start();
    _flashSize = this->get_flash_size();
    _pageSize = this->get_page_size();
    _lastSectorSize =
        this->get_sector_size(this->_flashStartAddress + this->_flashSize - 1);
    _flashEndAddress = this->_flashStartAddress + this->_flashSize;
    _sectorSize = this->get_sector_size(this->_flashEndAddress - 1);
    _lastSectorStart = this->_flashEndAddress - this->_lastSectorSize;
    TG_DEBUG_FILE_FUN_LINE(ok);
}
void LastSectionReaderWriter::WriteCurrentSetting() {
    if (_eraseLastSector() == true) {
        if (this->program(&current_setting, this->_lastSectorStart,
                          sizof(setting_t)) != 0) {
            TG_ERR_FILE_FUN_LINE();
        }
    }
    TG_DEBUG_FILE_FUN_LINE(ok);
}

void LastSectionReaderWriter::ReadCurrentSetting() {
    if (this->read(&current_setting, this->_lastSectorStart, sizof( setting_t)) != 0) {
        TG_ERR_FILE_FUN_LINE();
    }
    TG_DEBUG_FILE_FUN_LINE(ok);
}