
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#ifndef LINEFOLLOWERS_H
#define LINEFOLLOWERS_H

//should be at max 16 because of integer overflow
#define adc_SAMPLE_SIZE 10

// adc inputs for sample_linefollower parameter
#define adc_LEFT   0
#define adc_MIDDLE 1
#define adc_RIGHT  2

void init_linefollowers();

uint16_t sample_linefollower(uint8_t theOne);

#endif
