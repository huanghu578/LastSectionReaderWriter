#pragma once
#include "mbed.h"
#define MBED_ERR_FILE                 error("%s\n",__FILE__)
#define MBED_ERR_FUN                  error("%s\n",__func__)
#define MBED_ERR_LINE                 error("%d\n",__LINE__)
#define MBED_ERR_FILE_FUN_LINE(x)     error("%s,%s,%d:%s\n",__FILE__,__func__,__LINE__,#x)
#define MBED_DEBUG_FILE_FUN_LINE(x)   debug("%s,%s,%d:%s\n",__FILE__,__func__,__LINE__,#x)




