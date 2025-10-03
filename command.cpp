#include "command.h"
#include "hash.h"
string read_help(){
    return titleStr + std::string(help_str)  + std::string(help_str2) + std::string(remark_str1) + std::string(remark_str2);
}
bool read_block(string index, string& block_info) {
    if (canConvertToInt(index) &&
        (1 <= std::stoi(index)) && 
        (std::stoi(index) <= TYPE)) {
        int idx = std::stoi(index);
        auto mode=current_setting.blocks[idx - 1].mode ? string("1(lifetime license mode)"):string("0(pay-per-use mode)");
        block_info =
            "Block " + to_string(idx) +
            ": Mode:" + mode +
            ", Max Try:" + to_string(current_setting.blocks[idx - 1].max_try) +
            ", Current Try:" + to_string(current_setting.blocks[idx - 1].current_try) +
            ", Block Seed:xxx";
        return true;
    }
    return false;
}
bool read_response(string index, string query_code, uint64_t& response) {
    if (canConvertToInt(index) && (1 <= std::stoi(index)) &&
        (std::stoi(index) <= TYPE)) {
        int idx = std::stoi(index);
        if (current_setting.blocks[idx-1].mode==MODE0) {
            if (current_setting.blocks[idx - 1].current_try >
                current_setting.blocks[idx - 1].max_try) {//达到最大次数后，不可以使用，也不增加次数以造成无谓的写flash
                return false;
            }
            current_setting.blocks[idx-1].current_try+=1;
            write_current_setting();
        }
        uint32_t seed = djb2(current_setting.password.data());
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
    if ((current_password == string(current_setting.password.data(),
                                    current_setting.password.size())) &&
        (current_password.size() < MAX_BYTES_TO_PASSWORD) &&
        (new_password.size() < MAX_BYTES_TO_PASSWORD) &&
        (new_tip.size() < MAX_BYTES_TO_PASSWORD)) {
        if (current_password == new_password &&
            new_tip == current_setting.passwordTip.data()) {
            return false;  // 密码、提示前后相同，不应该写flash
        }
        
        const char* src = new_password.c_str();
        std::copy(src, src + new_password.length() + 1,
                  current_setting.password.begin());  // 包括'\0'
        const char* src2 = new_tip.c_str();
        std::copy(src2, src2 + new_tip.length() + 1,
                  current_setting.passwordTip.begin());  // 包括'\0'

        /* for (int i = 0; i < parameters[2].length(); i++) {
            current_setting.password[i] = parameters[2][i];
        }
        current_setting.password[parameters[2].length()] ='\0';
        for (int i = 0; i < parameters[3].length(); i++) {
            current_setting.passwordTip[i] =
                parameters[3][i];
        }
        current_setting.passwordTip[parameters[3].length()] = '\0';
        */

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
    if ((current_password == string(current_setting.password.data(),
                                    current_setting.password.size())) &&
        canConvertToInt(index) && canConvertToInt(max_try) &&
        canConvertToInt(seed) && (1 <= std::stoi(index)) &&
        (std::stoi(index) <= TYPE)) {
        if ((mode == "0" || mode == "1")) {
            int idx = std::stoi(index);
            uint8_t m = mode == "0" ? 0 : 255;
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

bool write_init_setting(){
    InitCurrentSetting();
    write_current_setting();
    return true;
}
