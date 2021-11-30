/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include <ios>
#include <iostream>
#include <iomanip>
#include "display.h"

typedef struct {
    float    voltage;   /* AD result of measured voltage */
    float    current;   /* AD result of measured current */
    int counter;   /* A counter value               */
} message_t;

static MemoryPool<message_t, 32> mpool;
static Queue<message_t, 32> queue;

void displaySendUpdateSensor(float volts, float amps, int cycles) {
    message_t *message = mpool.alloc();
    if(message) {
        message->voltage =  volts;
        message->current = amps;
        message->counter = cycles;
        queue.put(message);
    }
}

void displayThread(void)
{
    cout << "\033c" << "\033[?25l" << endl;
    std::cout << "\033[H" 
         << "Voltage:                    V\r\n"
         << "Current:                    A\r\n"
         << "Number of cycles\r\n";
    while (true) {
        osEvent evt = queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t *)evt.value.p;
            std::cout << "\033[1;21H" << std::fixed << std::setw( 8 ) << std::setprecision( 2 ) 
                 << (message->voltage);//<< "V\r\n" ;
//            printf("\nVoltage: %.2f V\n\r", message->voltage);
            std::cout << "\033[2;21H" << std::fixed << std::setw( 8 ) << std::setprecision( 2) 
                 << (message -> current); // << "A\r\n" ;
//            printf("Current: %.2f A\n\r", message->current);
            std::cout << "\033[3;18H" <</*std::dec << */ std::setw( 8 ) 
                 //<< std::showbase //<< std::setfill( '0' ) 
                 << (message -> counter) << std::endl ;
//            printf("Number of cycles: %u\n\r", message->counter);

            mpool.free(message);
        }
    }
}
