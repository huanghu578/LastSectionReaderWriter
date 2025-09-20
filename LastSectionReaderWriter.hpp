/// 读写MCU内最后一个section的工具
#pragma once
#include <inttypes.h>
#include <array>
#include "mbed.h"
#define LOCKED0                 0x00   // 已锁定
#define LOCKED255               0xFF // 未锁定
#define MODE0                   0x00 // 次数模式
#define MODE255                 0xFF // 正常模式
#define INIT_MAX_TRY            20
#define INIT_PASSWORD           "197001010000000"
#define INIT_TIP                "TIME_STAMP_OF_UNIX"
#define VID                     0x1234
#define PID                     0x4321
#define RID                     0x0001
#define VERSION                 202608
#define MAX_PASSWORD_TIME       10
#define MAX_BYTES_TO_PASSWORD   256

typedef struct {
    array<char, MAX_BYTES_TO_PASSWORD> password;
    array<char, MAX_BYTES_TO_PASSWORD> passwordTip;
    char locked;
    char mode;
    uint16_t max_try;
    uint16_t current_try;
} setting_t;


void ResetCurrentSetting();

class LastSectionReaderWriter : private FlashIAP {
   public:
    LastSectionReaderWriter();
    void WriteCurrentSetting();
    void ReadCurrentSetting();
   private:
    uint32_t _lastSectorStart;
    uint32_t _flashStartAddress;
    uint32_t _flashSize;
    uint32_t _pageSize;
    uint32_t _lastSectorSize;
    uint32_t _flashEndAddress;
    uint32_t _sectorSize;
    uint32_t _whereToWrite;
    bool _eraseLastSector();
};

extern setting_t current_setting;
