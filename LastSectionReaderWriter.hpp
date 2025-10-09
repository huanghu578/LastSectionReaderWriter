/// 读写MCU内最后一个section的工具
#pragma once
#include <inttypes.h>
#include <array>
#include <time.h>
#include <cstdio>
#include <string>
#include "TgMbedHelper.h"
#include "mbed.h"
#include "hash.h"
using namespace std;
extern const string INIT_PASSWORD;
extern const string INIT_TIP;
extern constexpr uint8_t MAX_BYTES_TO_PASSWORD;
extern constexpr int AUTO_INIT_CURRENT_SETTING_ID;
extern constexpr int VERSION;
extern constexpr uint8_t TYPE;  // 类型,不同档次产品的区分
extern constexpr uint16_t INIT_MAX_TRY;
extern constexpr uint8_t MODE_PAY_PER_USE;  // 次数模式
extern constexpr uint8_t MODE_LIFTIME;  // 正常模式
typedef struct {
    uint8_t mode;
    uint16_t max_try;
    uint16_t current_try;
    uint32_t block_seed;
} block_t;  // TODO:结合RAM，考虑在block_t中增加密码及其提示

typedef struct {
    string password;
    string passwordTip;
    uint32_t id;  // 作为自动初始化标记，当0xFFFFFFFF时，表示未初始化    
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
