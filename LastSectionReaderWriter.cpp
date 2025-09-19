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
    printf("Locked: %d\n", current_setting.locked);  
    printf("Mode: %d\n", current_setting.mode);
    printf("Max Try: %d\n", current_setting.max_try);
    printf("Current Try: %d\n", current_setting.current_try);
}

void ResetCurrentSetting() {
    current_setting = {INIT_PASSWORD, INIT_TIP, LOCKED255, MODE255,
                       INIT_MAX_TRY, 0};
    PrintCurrentSetting();
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
                          sizeof(setting_t)) != 0) {
            TG_ERR_FILE_FUN_LINE();
        }
    }
    PrintCurrentSetting();
    TG_DEBUG_FILE_FUN_LINE(ok);
}

void LastSectionReaderWriter::ReadCurrentSetting() {
    if (this->read(&current_setting, this->_lastSectorStart,
                   sizeof(setting_t)) != 0) {
        TG_ERR_FILE_FUN_LINE();
    }
    PrintCurrentSetting();
    TG_DEBUG_FILE_FUN_LINE(ok);
}