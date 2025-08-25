#ifndef _SETTING_H
#define _SETTING_H
#define LOCKED 1
#define NOT_LOCKED 255

typedef struct {
    char password[256];    
    char passwordTip[256];    
    char locked;        
}setting_t;

setting_t* readSettingFromLastSector();
bool writeSettingToLastSector(setting_t* setting);

extern setting_t* current_setting;
void PrintSetting(setting_t* st);
void ResetSetting(setting_t* st);

#endif