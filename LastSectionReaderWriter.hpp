/// 读写MCU内最后一个section的工具
#pragma once
#include <inttypes.h>
#include <array>
#include "mbed.h"
#define LOCKED 1
#define NOT_LOCKED 255
#define MAX_BYTES_TO_PASSWORD   (256)
#define MAX_BYTES_TO_READWRITE  (MAX_BYTES_TO_PASSWORD * 2 + 1)

typedef struct {
    array<char, MAX_BYTES_TO_PASSWORD> password;
    array<char, MAX_BYTES_TO_PASSWORD> passwordTip;
    char locked;
} setting_t;
extern setting_t current_setting;
void PrintCurrentSetting();
void ResetCurrentSetting();

class LastSectionReaderWriter : private FlashIAP {
   public:
    LastSectionReaderWriter();
    void LastSectionReaderWriter::WriteCurrentSetting();
    void LastSectionReaderWriter::ReadCurrentSetting();
   private:
    uint32_t _lastSectorStart;
    uint32_t _flashStartAddress;
    uint32_t _flashSize;
    uint32_t _pageSize;
    uint32_t _lastSectorSize;
    uint32_t _flashEndAddress;
    uint32_t _sectorSize;
    uint32_t _whereToWrite;
    bool _eraseOneSector(uint32_t startAddressOfSector);
};
