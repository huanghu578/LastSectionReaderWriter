#include "LastSectionReaderWriter.hpp"
#include "TgMbedHelper.h"
#include <time.h>
setting_t current_setting;

void PrintCurrentSetting() {
    printf("Password:%s \n", current_setting.password.data());    
    printf("Password Tip:%s \n", current_setting.passwordTip.data());    
    printf("Locked: %d\n", current_setting.locked);
    printf("ID: %d\n", current_setting.id);
    for (uint16_t i = 0; i < current_setting.blocks.size(); i++)
    {
        printf("Block %zu: Mode:%d, Max Try:%d, Current Try:%d, Block code:%d\n", i,
               current_setting.blocks[i].mode,
               current_setting.blocks[i].max_try,
               current_setting.blocks[i].current_try,
               current_setting.blocks[i].block_code);
    }
}

void ResetCurrentSetting() {
    current_setting.password =INIT_PASSWORD;
    current_setting.passwordTip = INIT_TIP;
    current_setting.locked = LOCKED255;
    current_setting.id = (uint32_t)time(NULL);
    for (uint16_t i = 0; i < current_setting.blocks.size(); i++)
    {
        current_setting.blocks[i] = {MODE255, INIT_MAX_TRY, 0, i};
    }
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
    if (sizeof(setting_t) > _lastSectorSize) {
        TG_ERR_FILE_FUN_LINE(sizeof(setting_t) > _lastSectorSize);
    }
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
    if (current_setting.id == AUTO_INIT_CURRENT_SETTING) {//自动初始化
        ResetCurrentSetting();
    }
    PrintCurrentSetting();
    TG_DEBUG_FILE_FUN_LINE(ok);
}