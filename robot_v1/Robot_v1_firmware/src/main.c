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
					drive(100,-100);
					send_string("w", 2);
					shiftreg_send(sr_W);
					break;
				case 'a':
					drive(-100,-100);
					send_string("a", 2);
					shiftreg_send(sr_A);
					break;
				case 's':
					drive(-100,100);
					send_string("s", 2);
					shiftreg_send(sr_S);
					break;
				case 'd':
					drive(100,100);
					send_string("d", 2);
					shiftreg_send(sr_D);
					break;
				case '0':
					drive(0,0);
					send_string("Exiting remote drive", sizeof("Exiting remote drive"));
					return;
					break;
				case 'p':
					// the printf function prints to the usb serial connection NOT the bluetooth connection
					printf("left = %u | middle = %u | right = %u \n", 
					sample_linefollower(adc_LEFT), sample_linefollower(adc_MIDDLE), sample_linefollower(adc_RIGHT));
					break;
				default:
					drive(0,0);
					send_string("x", 3);
					shiftreg_send(sr_X);
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
    shiftreg_init();
    init_linefollowers();
    
    bool currentlydriving = false;
    bool testmode = false;
    
    bool left;
    bool middle;
    bool right;
    bool lastleft;
    bool lastmiddle;
    bool lastright;
    
	while (1) {
		
		// check for new controll char
		if (!pio_sm_is_rx_fifo_empty(pio0,sm3)){
			char recieved = (char) pio_sm_get(pio0, sm3);
			uint8_t i = 0;
			switch (recieved) {
				case 'S':
					send_string("START driving\n", sizeof("START driving\n"));
					currentlydriving = true;
					break;
				case 'D':
					send_string("REMOTE driving\n", sizeof("REMOTE driving\n"));
					remotedrive();
					break;
				case 'P':
					send_string("PAUSing\n", sizeof("PAUSing\n"));
					drive(0,0);
					while (pio_sm_is_rx_fifo_empty(pio0,sm3)){
						shiftreg_send(pauselight[i%sr_PAUSELIGHT_PERIOD]);
						i++;
						sleep_ms(50);
					}
					break;
				case 'T':
					send_string("TROUBLEing\n", sizeof("TROUBLEing\n"));
					drive(100,100);
					while (pio_sm_is_rx_fifo_empty(pio0,sm3)){
						shiftreg_send(paniclight[i%sr_PANICLIGHT_PERIOD]);
						i++;
						sleep_ms(50);
					}
					break;
				case 't':
					testmode = !testmode;
					if (testmode) {
						currentlydriving = true; 
					} else {
						currentlydriving = false; 
					}
					break;
				default:
					drive(0,0);
					send_string("x", 2);
					shiftreg_send(sr_E | (sr_R << sr_RIGHTSIDE));
			}
		}
	
	if (currentlydriving) {
		
		left   = sample_linefollower(adc_LEFT)   < THREASHOLD;
		middle = sample_linefollower(adc_MIDDLE) < THREASHOLD;
		right  = sample_linefollower(adc_RIGHT)  < THREASHOLD;
		
		shiftreg_send(
			((left)?sr_LEFT:0)|
			((middle)?sr_MIDDLE:0)|
			((right)?sr_RIGHT:0)
		);
		
		if (testmode) {
			printf("left = %u | middle = %u | right = %u \n", 
					sample_linefollower(adc_LEFT), sample_linefollower(adc_MIDDLE), sample_linefollower(adc_RIGHT));
			;
		}else if (middle) {
			lastleft = false;
			lastright = false;
			driveForeward();
			send_string("|\n",3);
		} else if (right) {
			lastleft = false;
			lastright = true;
			turnRight();
			send_string(">\n",3);
		} else if (left) {
			lastleft = true;
			lastright = false;
			turnLeft();
			send_string("<\n",3);
		} else if (lastleft) {
			rotateLeft();
			send_string("\\\n",3);
		} else if (lastright) {
			rotateRight();
			send_string("/\n",3);
		} else {
			driveForeward();
			send_string("|\n",3);
		}
		sleep_ms(10);
	}
	
	}
    
}
