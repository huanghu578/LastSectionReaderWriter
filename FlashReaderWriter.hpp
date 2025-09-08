//读写MCU内部FLASH的工具，因为内部擦除是擦除某个sector，所以，读写的开始地址应该在该扇区起始，以确保读写范围全部落在此扇区内。
#ifndef __FLASH__MEMORY__H
#define __FLASH__MEMORY__H
#include "mbed.h"
#include <inttypes.h>
#define MAX_BYTES_TO_READWRITE (256+256+1)

typedef struct {
    uint8_t bytes[MAX_BYTES_TO_READWRITE];
    uint32_t len;
}U8ToReadWrite;

typedef struct {
    char bytes[MAX_BYTES_TO_READWRITE];
    uint32_t len;
}chars2read_write_t;

class FlashReaderWriter : private FlashIAP {
    public:
        FlashReaderWriter();
        uint32_t lastSectorStart;
        bool writeU8sToLastSector(U8ToReadWrite data);
        U8ToReadWrite *readU8sFromLastSector(uint32_t lenToRead=MAX_BYTES_TO_READWRITE);
        bool writeCharsToLastSector(chars2read_write_t data);
        chars2read_write_t *readCharsFromLastSector(uint32_t lenToRead=MAX_BYTES_TO_READWRITE);
        void PrintSetting(const chars2read_write_t& st);

    private:
        uint32_t _flashStartAddress;
        uint32_t _flashSize;
        uint32_t _pageSize;
        uint32_t _lastSectorSize;
        uint32_t _flashEndAddress;
        uint32_t _sectorSize;
        uint32_t _whereToWrite;
        bool   _eraseOneSector(uint32_t startAddressOfSector);
};
#endif