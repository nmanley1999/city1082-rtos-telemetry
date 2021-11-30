#include "mbed.h"
#include "display.h"

/* Send Thread */

typedef struct {
    float    voltage;   /* AD result of measured voltage */
    float    current;   /* AD result of measured current */
    int counter;   /* A counter value               */
} message_t;

static  MemoryPool<message_t, 16> mpool;
static Queue<message_t, 32> queue;


void sendThread(void)
{
    uint32_t i = 0;
    while (true) {
        i++; // fake data update
        message_t *message = mpool.try_alloc();
        message->voltage = (i * 0.1) * 33;
        message->current = (i * 0.1) * 11;
        message->counter = i;
        displaySendUpdateSensor(message->voltage, message->current, message->counter);
        mpool.free(message);
        ThisThread::sleep_for(1s);
    }
}
