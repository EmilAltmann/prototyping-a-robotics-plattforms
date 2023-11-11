 
#ifndef sm3
#define sm3 3
#endif

#include "pico/stdlib.h"
#include "btstack_run_loop.h"
#include "btstack/bluelib.h"
#include "hardware/pio.h"
#include "motors.h"
#include "shiftreg.pio.h"
#include "linefollowers.h"

/// the threashold for the three infred line sensors
/// can be between 0 and 4095
#define THREASHOLD 2500 

/// the loop conter for the LED animations
/// can be between 0 and 4095
#define TIMEINTERVALL 1000000
