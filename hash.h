#include <time.h>
#include <cstdint>
#include <string>

using namespace std;

uint32_t MurmurHash2(const string& str,uint32_t seed=0);
uint32_t djb2(const string& str);
uint32_t fnv1a(const string& str);

//----------------------------------------------------------------------------open
bool read_response(uint8_t block_index,string query_code,uint64_t &response);
const string GetGuid();





