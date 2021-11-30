#include "mbed.h"
#include "display.h"
#include <cmath>

/* Send Thread */

typedef struct {
    float    temperature;  /* AD result of measured voltage */
    float    lightLevel;   /* AD result of measured current */
    int      cycles;       /* A counter value               */
} message_t;

static  MemoryPool<message_t, 16> mpool;


void sendThread(void)
{
    uint32_t i = 0;
    while (true) {
        i++; // fake data update
        message_t *message = mpool.try_alloc();
        message->temperature = fmod((i * 0.1f) * 33.0f, 40.0f);
        message->lightLevel = fmod((i * 0.1f) * 11.0f, 100);
        message->cycles = i;
        displaySendUpdateSensor(message->temperature, message->lightLevel, message->cycles);
        mpool.free(message);
        ThisThread::sleep_for(1s);
    }
}
