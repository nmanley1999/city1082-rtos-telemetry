#include "mbed.h"
#include "display.h"
#include <cmath>

/* Send Thread */

void sendThread(void)
{
    float    temp;  /* AD result of measured voltage */
    float    lightLev;   /* AD result of measured current */
    int      cycles;       /* A counter value               */
    uint32_t i = 0;
    while (true) {
        i++; // fake data update
        temp = fmod((i * 0.1f) * 3.3f, 40);
        lightLev = fmod((i * 0.1f) * 5.5f, 100);
        cycles = i;
        displaySendUpdateSensor(temp, lightLev, cycles);
        ThisThread::sleep_for(1s);
    }
}
