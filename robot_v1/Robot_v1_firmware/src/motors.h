#ifndef MOTORS_H
#define MOTORS_H

#include "pico/stdlib.h"
#include "stdio.h"
#include "hardware/pio.h"
#include "motors.pio.h"

/// Statemashines for running the steppers
#define sm0 0
#define sm1 1

/// between 0 and 5535!
#define MINDIV 5000

void initDrive();

static int motor_offset;

void drive(int left, int right);

void driveForeward();
void driveBackwrd();
void turnLeft();
void rotateLeft();
void turnRight();
void rotateRight();

#endif
