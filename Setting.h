#ifndef _SETTING_H
#define _SETTING_H
#include "FlashReaderWriter.hpp"

#define LOCKED 1
#define NOT_LOCKED 255

typedef union {
    struct {
        char password[256];    
        char passwordTip[256];    
        char locked;
    };
    char bytes[MAX_BYTES_TO_READWRITE];         
}setting_t;

setting_t* readSettingFromLastSector();
bool writeSettingToLastSector(const setting_t& setting);

extern setting_t current_setting;
void PrintSetting(const setting_t& st);
void ResetSetting(setting_t& st);

#endif