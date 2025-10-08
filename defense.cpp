#include "defense.h"
int current_time;
void delay_for_defense(){
    if (current_time < TIME1) {
        ThisThread::sleep_for(DURATION1);
    } else if (current_time < TIME2) {
        ThisThread::sleep_for(DURATION2);
    } else if (current_time < TIME3) {
        ThisThread::sleep_for(DURATION3);
    } else {
        ThisThread::sleep_for(DURATION4);
    }
}