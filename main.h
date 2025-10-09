#pragma once
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <type_traits>
#include "FlashRDP.h"
#include "LastSectionReaderWriter.hpp"
#include "Led.h"
#include "SerialReader.h"
#include "StringExt.h"
#include "TgMbedHelper.h"
#include "command.h"
#include "defense.h"
#include "hash.h"
#include "mbed.h"
#include "serial_str.h"

void WrongCommand();
void OkCommand(string commane);