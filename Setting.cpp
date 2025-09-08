#include <cstdint>
#include "Setting.h"
#include "flash_data.h"
#include "main.h"

setting_t* readSettingFromLastSector(){
    auto internalFlash=new FlashReaderWriter();
    auto temp=internalFlash->readCharsFromLastSector();    
    if (temp->len!=sizeof(setting_t)) {
        TG_ERR_FILE_FUN_LINE();
    }    
    TG_DEBUG_FILE_FUN_LINE(read chars2read_write_t:); 
    internalFlash->PrintSetting(*temp);    
    auto setting=(setting_t*) malloc(sizeof(setting_t));
    memset((void*)setting, 0, sizeof(setting_t));
    std::copy(temp->bytes, temp->bytes + MAX_BYTES_TO_READWRITE, setting->bytes);
    TG_DEBUG_FILE_FUN_LINE(to setting_t:); 
    PrintSetting(*setting);
    delete internalFlash;
    free(temp);
    return setting;
}

bool writeSettingToLastSector(const setting_t& setting){
    chars2read_write_t temp={0};
    std::copy(setting.bytes, setting.bytes + MAX_BYTES_TO_READWRITE, temp.bytes);
    temp.len=MAX_BYTES_TO_READWRITE;
    printf("write chars2read_write_t:");
    auto internalFlash=new FlashReaderWriter();
    internalFlash->PrintSetting(temp);
    bool b=false;
    if (internalFlash->writeCharsToLastSector(temp)){        
        b= true;
    }
    delete internalFlash;    
    return b;
}



setting_t current_setting;
void PrintSetting(const setting_t& st){
    printf("password:%s,tip:%s,locked:%d\n",st.password,st.passwordTip,st.locked);
}
void ResetSetting(setting_t& st){    
    st={INIT_PASSWORD,INIT_TIP,NOT_LOCKED};    
    printf("ResetSetting(setting_t& st){:\n");
    PrintSetting(st);//password:1970b01ww01,tip:BirthdayWith1_2,locked:255
}