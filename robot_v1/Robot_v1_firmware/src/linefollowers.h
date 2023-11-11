
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#ifndef LINEFOLLOWERS_H
#define LINEFOLLOWERS_H

//should be at max 16 because of interger overflow
#define adc_SAMPLE_SIZE 10

/// the variables for the sample_linefollers function
#define adc_LEFT   2
#define adc_MIDDLE 0
#define adc_RIGHT  1

void init_linefollowers();

uint16_t sample_linefollower(uint8_t theOne);

#endif
