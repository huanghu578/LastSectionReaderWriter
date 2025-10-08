#pragma once
#include <chrono>
#include "mbed.h"
constexpr uint8_t TIME1 = 5;
constexpr uint8_t TIME2 = 10;
constexpr uint8_t TIME3 = 20;
constexpr uint8_t TIME4 = 30;
constexpr std::chrono::milliseconds DURATION1(1000);
constexpr std::chrono::milliseconds DURATION2(5000);
constexpr std::chrono::milliseconds DURATION3(25000);
constexpr std::chrono::milliseconds DURATION4(125000);
extern int current_time;
void delay_for_defense();