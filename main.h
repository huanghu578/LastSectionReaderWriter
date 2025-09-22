#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <type_traits>
#include "LastSectionReaderWriter.hpp"
#include "StringExt.h"
#include "command.h"
#include "hash.h"
#include "serial_str.h"
#ifdef __MBED__
#include "FlashRDP.h"
#include "Led.h"
#include "TgMbedHelper.h"
#include "USBSerial.h"
#include "mbed.h"
#endif