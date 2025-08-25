#include <cstdint>
#include "Setting.h"
#include "flash_data.h"
#include "FlashReaderWriter.hpp"
#include "main.h"

setting_t* readSettingFromLastSector(){
    auto internalFlash=new FlashReaderWriter();
    auto temp=internalFlash->readCharsFromLastSector();    
    if (temp->len!=sizeof(setting_t)) {
        TG_ERR_FILE_FUN_LINE;
    }
    auto setting=(setting_t*) malloc(sizeof(setting_t));
    memset((void*)setting, 0, sizeof(setting_t));
    std::copy(temp->bytes, temp->bytes + sizeof(setting->password), setting->password);
    std::copy(temp->bytes+sizeof(setting->password), 
        temp->bytes+sizeof(setting->password)+sizeof(setting->passwordTip), 
        setting->passwordTip);    
    setting->locked=temp->bytes[MAX_BYTES_TO_READWRITE-1];
    delete internalFlash;
    free(temp);
    return setting;
}

bool writeSettingToLastSector(setting_t* setting){
    CharsToReadWrite temp={0};
    std::copy(setting->password, setting->password + sizeof(setting->password), temp.bytes);
    std::copy(setting->passwordTip, 
        setting->passwordTip + sizeof(setting->passwordTip), 
        temp.bytes + sizeof(setting->password));    
    temp.bytes[MAX_BYTES_TO_READWRITE-1]=setting->locked;
    auto internalFlash=new FlashReaderWriter();
    bool b=false;
    if (internalFlash->writeCharsToLastSector(temp)){        
        b= true;
    }
    delete internalFlash;    
    return b;
}



setting_t* current_setting;
void PrintSetting(setting_t* st){
    printf("password:%s,tip:%s,locked:%d\n",st->password,st->passwordTip,st->locked);
}
void ResetSetting(setting_t* st){
    auto setting=(setting_t*) malloc(sizeof(setting_t));        
    char init_password[]=INIT_PASSWORD;
    char init_tip[]=INIT_TIP;
    std::copy(init_password, init_password + sizeof(init_password), setting->password);
    std::copy(init_tip, init_tip + sizeof(init_tip), setting->passwordTip);
    setting->locked=NOT_LOCKED;
    st=setting;
}