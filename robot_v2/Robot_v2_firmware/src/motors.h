#ifndef MOTORS_H
#define MOTORS_H

#include "pico/stdlib.h"
#include "stdio.h"
#include "hardware/pwm.h"

/// IO pins for the stepper drivers
#define RD 0
#define RS 1
#define LD 2
#define LS 3

/// Speed levels for driving around
#define HIGHSPEED 1
#define LOWSPEED 10

/// clk div for max speed
#define MAXSPEED 1
#define MINSPEED 256

/// set for the max speed if clk div is set to 1 and NEMA17 motors
#define WRAPVAL 32000

/// PWM slices and channels
static uint slice_L;
static uint channel_L;
static uint slice_R;
static uint channel_R;

void set_speed(uint8_t speed_l, uint8_t speed_r);

void initDrive();

void driveForeward();
void driveBackwrd();
void rotateLeft();
void rotateRight();
void turnLeft();
void turnRight();
void stop();

#endif
