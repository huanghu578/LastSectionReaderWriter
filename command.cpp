#include "command.h"
#include "hash.h"
bool write_init_setting(){
    InitCurrentSetting();
    write_current_setting();
    return true;
}
bool block(string index, string& block_info) {
    if (canConvertToInt(index) &&
        (1 <= std::stoi(index)) && 
        (std::stoi(index) <= TYPE)) {
        int idx = std::stoi(index);
        auto mode=current_setting.blocks[idx - 1].mode ? string("1(lifetime license mode)") : string("0(pay-per-use mode)");
        block_info =
            "Block " + index +
            ": Mode:" + mode +
            ", Max Try:" + to_string(current_setting.blocks[idx - 1].max_try) +
            ", Current Try:" + to_string(current_setting.blocks[idx - 1].current_try) +
            ", Block Seed:xxx";
        return true;
    }
    return false;
}
bool query(string index, string query_code, uint64_t& response) {
    if (canConvertToInt(index) && (1 <= std::stoi(index)) &&
        (std::stoi(index) <= TYPE)) {
        int idx = std::stoi(index);
        if (current_setting.blocks[idx - 1].mode == MODE_PAY_PER_USE) {
            if (current_setting.blocks[idx - 1].current_try >
                current_setting.blocks[idx - 1].max_try) {//达到最大次数后，不可以使用，也不增加次数以造成无谓的写flash
                return false;
            }
            current_setting.blocks[idx-1].current_try+=1;
            write_current_setting();
        }
        uint32_t seed = djb2(string(current_setting.password.data()));
        uint32_t p1 = MurmurHash2(query_code, seed);
        uint32_t p2 =
            fnv1a(to_string(current_setting.blocks[idx-1].block_seed));
        uint64_t p3 = p1 + p2;
        p3 += VERSION;
        p3 += (p3 % 100);
        response = p3;
        return true;
    }
    return false;
}
bool write_dongle(string current_password,
                  string new_password,
                  string new_tip) {
    if (current_password.size()>MAX_BYTES_TO_PASSWORD || 
        new_password.size()>MAX_BYTES_TO_PASSWORD || 
        new_tip.size()>MAX_BYTES_TO_PASSWORD) {
        return false;
    }
    if (current_password == new_password &&
        new_tip == string(current_setting.passwordTip.data())) {
        return false;  // 密码、提示前后相同，不应该写flash
    }
    if (current_password == string(current_setting.password.data())) {
        std::copy_n(new_password.begin(), new_password.size(), current_setting.password.begin());
        current_setting.password[new_password.size()] = '\0'; // 确保字符串以null结尾
        std::copy_n(new_tip.begin(), new_tip.size(), current_setting.passwordTip.begin());
        current_setting.passwordTip[new_tip.size()] = '\0'; // 确保字符串以null结尾

        write_current_setting();
        return true;
    }
    return false;
}

bool write_block(string current_password,
                 string index,
                 string mode,
                 string max_try,
                 string seed) {
    if (current_password.size()>MAX_BYTES_TO_PASSWORD) {
        return false;
    }
    if ((current_password == string(current_setting.password.data())) &&
        canConvertToInt(index) && canConvertToInt(max_try) &&
        canConvertToInt(seed) && (1 <= std::stoi(index)) &&
        (std::stoi(index) <= TYPE)) {
        if ((mode == "0" || mode == "1")) {
            int idx = std::stoi(index);
            uint8_t m = mode == "0" ? MODE_PAY_PER_USE : MODE_LIFTIME;
            int mt = std::stoi(max_try);
            int s = std::stoi(seed);
            if(current_setting.blocks[idx-1].mode==m &&
               current_setting.blocks[idx-1].max_try==mt &&
               current_setting.blocks[idx-1].block_seed==s){
                return false;//block参数前后相同，不应该写flash
            }
            current_setting.blocks[idx - 1].mode = m;
            current_setting.blocks[idx - 1].max_try = mt; 
            current_setting.blocks[idx - 1].block_seed = s;
            current_setting.blocks[idx - 1].current_try = 0; //重置尝试次数
            write_current_setting();
            return true;
        }
    }
    return false;
}
