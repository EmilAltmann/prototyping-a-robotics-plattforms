/*
 * thanks to:
 * Copyright (c) 2022 Mr. Green's Workshop https://www.MrGreensWorkshop.com
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include "main.h"

/// driving the robot remotely with single character input
/// w a s d for movement and 0 for retuning to the main loop
/// p for reading the line detectors and any key to pause
void remotedrive() {
	
	while (1) {
		//check if another char is send via bluetooth
		if (!pio_sm_is_rx_fifo_empty(pio0,sm3)){
			char recieved = (char) pio_sm_get(pio0, sm3);
			switch (recieved) {
				case 'w':
					driveForeward();
					send_string("w", 2);
					break;
				case 'a':
					driveLeft();
					send_string("a", 2);
					break;
				case 'x':
					driveBackward();
					send_string("x", 2);
					break;
				case 'd':
					driveRight();
					send_string("d", 2);
					break;
				case 'q':
					driveForewardLeft();
					send_string("q", 2);
					break;
				case 'e':
					driveForewardRight();
					send_string("e", 2);
					break;
				case 'y':
					driveBackwardLeft();
					send_string("y", 2);
					break;
				case 'c':
					driveBackwardRight();
					send_string("c", 2);
					break;
				case 'i':
					turnLeft();
					send_string("i", 2);
					break;
				case 'o':
					turnRight();
					send_string("o", 2);
					break;
				case 'k':
					rotateLeft();
					send_string("k", 2);
					break;
				case 'l':
					rotateRight();
					send_string("l", 2);
					break;
				case '1':
					lfMotor();
					send_string("1", 2);
					break;
				case '2':
					rfMotor();
					send_string("2", 2);
					break;
				case '3':
					lbMotor();
					send_string("3", 2);
					break;
				case '4':
					rbMotor();
					send_string("4", 2);
					break;
				default:
					stop();
					send_string("x", 3);
			}
		}
		
	}
}

/// For controlling the Robot use:
/// S for autonomus drving
/// D for remote driving
/// P for pausing if currently autonomusly driving
/// T for Troubleing (turning on one spot)
/// t for testmode (sending telemtry over usb)
int main() {
    stdio_init_all();
    setupBluetoothTransmission();
    initDrive();
    
	remotedrive();
}

