#include "LastSectionReaderWriter.hpp"
#include "TgMbedHelper.h"
setting_t current_setting;
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
    if (_eraseOneSector(lastSectorStart) == true) {
        if (this->program(&current_setting, 
                            _lastSectorStart,
                            MAX_BYTES_TO_READWRITE) != 0) {
            TG_ERR_FILE_FUN_LINE();
        }
    }
    TG_DEBUG_FILE_FUN_LINE(ok);
}

void LastSectionReaderWriter::ReadCurrentSetting() {    
    if (this->read(&current_setting, _lastSectorStart,
                   MAX_BYTES_TO_READWRITE) != 0) {
        TG_ERR_FILE_FUN_LINE();
    }
    TG_DEBUG_FILE_FUN_LINE(ok);
    return data;
}

bool LastSectionReaderWriter::_eraseOneSector(uint32_t startAddressOfSector) {
    if (this->erase(startAddressOfSector, this->_sectorSize) != 0) {
        TG_ERR_FILE_FUN_LINE();
    }
    return true;
}

void LastSectionReaderWriter::PrintSetting(
    array<char, MAX_BYTES_TO_READWRITE> data) {
    for (auto c : data) {
        printf("%c", c);
    }
    printf("\n");
}