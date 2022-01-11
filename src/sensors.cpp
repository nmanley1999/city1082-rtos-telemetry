#include "mbed.h"
#include "display.h"
#include <cmath>

#define THERM_GND P10_0
#define THERM_VCC P10_3
#define THERM_OUT P10_1


/* Reference resistor in series with the thermistor is of 10kohm */
#define R_REFERENCE                 (float)(10000)

/* A constant of NCP18XH10F03RB thermistor is */
#define A_COEFF                     (float)(0.000932679f)
/* A constant of NCP18XH10F03RB thermistor is */
#define B_COEFF                     (float)(0.000248772f)
/* A constant of NCP18XH10F03RB thermistor is */
#define C_COEFF                     (float)(0.0000002041094f)

/* Zero Kelvin in degree C */
#define ABSOLUTE_ZERO


AnalogIn tempVoltage(THERM_OUT);
/* Send Thread */
float readTEmp();

void sendThread(void)
{
    // this is for reading the thermistor
    DigitalOut ground(THERM_GND);
    DigitalOut vcc(THERM_VCC);
    AnalogIn tempVoltage(THERM_OUT);
    // initialise thermistor power
    vcc= true;
    ground = false;

        float    temp;  // AD result of measured voltage 
    float    lightLev;   // AD result of measured current
    int      cycles;       // A counter value               
    uint32_t i = 0;
    while (true) {
        i++; // fake data update
        temp = tempVoltage.read() * 3.3f; 
        lightLev = fmod((i * 0.1f) * 5.5f, 100);
        cycles = i;
        displaySendUpdateSensor(temp, lightLev, cycles);
        ThisThread::sleep_for(1s);
        
    }
}

float readTemp () {
    float temperature;
    float voltRead = tempVoltage.read() * 2.4;
    float refCurrent = voltage/R_REFERENCE;
    float resTherm = (3.3f - voltRead) / refCurrent;
    float lnRt = log(resTherm);
    float stEqn = A_COEFF + B_COEFF*lnRt + C_COEFF*powe(lnRt, 3);
    temperature = (1/stEqn) + ABSOLUTE_ZERO +0.05

    return temperature;
}
