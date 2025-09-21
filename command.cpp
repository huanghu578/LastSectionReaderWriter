#include "command.h"
#include "hash.h"
string read_help(){
    return titleStr + std::string(help_str)  + std::string(help_str2) + std::string(remark_str1) + std::string(remark_str2);
}
string read_tip(){
    return current_setting.passwordTip.data();
}
uint32_t read_id(){
    return current_setting.id;
}
uint32_t read_version(){
    return VERSION;
}
uint8_t read_count(){
    return TYPE;
}

bool read_block(uint8_t block_index,block_t &b){
    if(block_index < TYPE) {
        b = current_setting.blocks[block_index];
        return true;
    }
    return false;
}

bool read_response(uint8_t block_index,string query_code,uint64_t &response){
    uint32_t seed=djb2(current_setting.password.data());
    uint32_t p1=MurmurHash2(query_code,seed);
    uint32_t p2=fnv1a(to_string(current_setting.blocks[block_index].block_code));
    uint64_t p3=p1+p2;
    p3+=VERSION;                   
    p3+=(p3%100);  
    response=p3;
    return true;
}

#ifdef __MBED__
bool write_block(uint8_t block_index,block_t b){
    if(block_index < TYPE) {
        current_setting.blocks[block_index] = b;
        auto f=new LastSectionReaderWriter();
        f->WriteCurrentSetting();
        delete f;
        return true;
    }
    return false;
}

bool write_password(string old_password,string new_password,string new_tip){
    if((old_password ==string(current_setting.password.data(), current_setting.password.size())) &&
       (new_password.size() < MAX_BYTES_TO_PASSWORD) &&
       (new_tip.size() < MAX_BYTES_TO_PASSWORD))
    {
        const char* src = new_password.c_str();        
        std::copy(src, src + new_password.length() + 1, current_setting.password.begin()); // 包括'\0'
        const char* src2 = new_tip.c_str();        
        std::copy(src2, src2 + new_tip.length() + 1, current_setting.passwordTip.begin()); // 包括'\0'
        auto f=new LastSectionReaderWriter();
        f->WriteCurrentSetting();
        delete f;
        return true;
    }
    return false;
}

bool write_init_setting(){
    InitCurrentSetting();
    auto f=new LastSectionReaderWriter();
    f->WriteCurrentSetting();
    delete f;
    return true;
}
#endif
