#ifndef MOTORS_H
#define MOTORS_H

#include "pico/stdlib.h"
#include "stdio.h"
#include "hardware/pwm.h"

/// IO pins for the stepper drivers
#define LBD 2
#define LBS 3
#define RBD 4
#define RBS 5
#define LFD 6
#define LFS 7
#define RFD 8
#define RFS 9

/// Speed levels for driving around
#define HIGHSPEED 1
#define LOWSPEED 10

/// clk div for max speed
#define MAXSPEED 1
#define MINSPEED 256

/// set for the max speed if clk div is set to 1 and NEMA17 motors
#define WRAPVAL 32000

/// PWM slices and channels
static uint slice_LF;
static uint channel_LF;
static uint slice_RF;
static uint channel_RF;
static uint slice_LB;
static uint channel_LB;
static uint slice_RB;
static uint channel_RB;

void set_speed(int8_t speed_lf, int8_t speed_rf, int8_t speed_lb, int8_t speed_rb);

void initDrive();

void lfMotor();
void rfMotor();
void lbMotor();
void rbMotor();

void driveForeward();
void driveBackward();
void rotateLeft();
void rotateRight();
void turnLeft();
void turnRight();
void driveRight();
void driveLeft();
void driveForewardRight();
void driveForewardLeft();
void driveBackwardRight();
void driveBackwardLeft();
void stop();

#endif
