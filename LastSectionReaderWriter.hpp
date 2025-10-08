/// 读写MCU内最后一个section的工具
#pragma once
#include <inttypes.h>
#include <array>
#include <time.h>
#include <cstdio>
#include <string>
#include "TgMbedHelper.h"
#include "mbed.h"

constexpr uint8_t MAX_BYTES_TO_PASSWORD=255;
constexpr int AUTO_INIT_CURRENT_SETTING_ID=0xFFFFFFFF;
constexpr uint16_t VID=0x1234;
constexpr uint16_t PID=0x4321;
constexpr uint16_t RID=0x0001;
constexpr int VERSION=202608;
//constexpr uint8_t LOCKED0= 0x00;    // 已锁定
//constexpr uint8_t LOCKED255= 0xFF;  // 未锁定
//constexpr uint8_t MAX_PASSWORD_TIME=10;
constexpr uint8_t TYPE=20;  // 类型,不同档次产品的区分
constexpr uint16_t INIT_MAX_TRY=20;
constexpr uint8_t MODE0= 0x00;      // 次数模式
constexpr uint8_t MODE255=0xFF;    // 正常模式


using namespace std;
typedef struct {
    uint8_t mode;
    uint16_t max_try;
    uint16_t current_try;
    uint32_t block_seed;
} block_t;  // TODO:结合RAM，考虑在block_t中增加密码及其提示

typedef struct {
    array<char, MAX_BYTES_TO_PASSWORD> password;
    array<char, MAX_BYTES_TO_PASSWORD> passwordTip;
    uint32_t id;  // 作为自动初始化标记，当0xFFFFFFFF时，表示未初始化
    //uint8_t locked;
    //uint8_t password_try_time;
    array<block_t, TYPE> blocks;  // 用block的数量来区分档次
} setting_t;
void InitCurrentSetting();
void write_current_setting();
void read_current_setting();

extern setting_t current_setting;


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
