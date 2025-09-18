// 读写MCU内部FLASH的工具，因为内部擦除是擦除某个sector，所以，读写的开始地址应该在该扇区起始，以确保读写范围全部落在此扇区内。
#pragma once
#include <inttypes.h>
#include <array>
#include "mbed.h"
#define MAX_BYTES_TO_READWRITE (1024)  // 1 K

class FlashReaderWriter : private FlashIAP {
   public:
    FlashReaderWriter();
    bool writeCharsToLastSector(array<char, MAX_BYTES_TO_READWRITE> data);
    array<char, MAX_BYTES_TO_READWRITE> readCharsFromLastSector();    
    PrintSetting(array<char, MAX_BYTES_TO_READWRITE> data);

   private:
    uint32_t lastSectorStart;
    uint32_t _flashStartAddress;
    uint32_t _flashSize;
    uint32_t _pageSize;
    uint32_t _lastSectorSize;
    uint32_t _flashEndAddress;
    uint32_t _sectorSize;
    uint32_t _whereToWrite;
    bool _eraseOneSector(uint32_t startAddressOfSector);
};
