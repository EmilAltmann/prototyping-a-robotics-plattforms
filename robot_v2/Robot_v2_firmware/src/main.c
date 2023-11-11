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
	
	uint32_t timestamp = TIMEINTERVALL; 
	
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
					turnLeft();
					send_string("a", 2);
					break;
				case 's':
					driveBackwrd();
					send_string("s", 2);
					break;
				case 'd':
					turnRight();
					send_string("d", 2);
					break;
				case 'q':
					rotateLeft();
					send_string("q", 2);
					break;
				case 'e':
					rotateRight();
					send_string("e", 2);
					break;
				case '0':
					stop();
					send_string("Exiting remote drive", sizeof("Exiting remote drive"));
					return;
					break;
				case 'p':
					// the printf function prints to the usb serial connection NOT the bluetooth connection
					printf("left = %u | middle = %u | right = %u \n", 
					sample_linefollower(adc_LEFT), sample_linefollower(adc_MIDDLE), sample_linefollower(adc_RIGHT));
					break;
				default:
					stop();
					send_string("x", 3);
			}
		}
		
		// blink the bargraphs to show if the robot is remote controlled
		if (timestamp == TIMEINTERVALL/2) {
			shiftreg_send(1<<9);
		} else if (timestamp == 2*TIMEINTERVALL/2) {
			shiftreg_send(1<<19);
		} else if (timestamp == 3*TIMEINTERVALL/2) {
			shiftreg_send(1<<29);
		} else if (timestamp == 4*TIMEINTERVALL/2) {
			shiftreg_send(1<<19);
			timestamp = 0;
		}
		timestamp++;
		
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
    
    uint16_t leftVal = 0;
    uint16_t middleVal = 0;
    uint16_t rightVal = 0;
    
    uint32_t shiftVal = 0;
    
    uint32_t timestamp = TIMEINTERVALL; 
    
    bool left;
    bool middle;
    bool right;
    bool lastleft;
    bool lastmiddle;
    bool lastright;
    
	while (1) {
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
					stop();
					while (pio_sm_is_rx_fifo_empty(pio0,sm3)){
						i++;
						sleep_ms(50);
					}
					break;
				case 'T':
					send_string("TROUBLEing\n", sizeof("TROUBLEing\n"));
					while (pio_sm_is_rx_fifo_empty(pio0,sm3)){
						shiftreg_send(i);
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
					stop();
					testmode = false;
					currentlydriving = false;
					shiftreg_clear();
					send_string(":-(", 4);
			}
		}
	
	if (testmode || currentlydriving) {
		leftVal = sample_linefollower(adc_LEFT);
		middleVal = sample_linefollower(adc_MIDDLE);
		rightVal = sample_linefollower(adc_RIGHT);
		
		left   = leftVal   < THREASHOLD;
		middle = middleVal < THREASHOLD;
		right  = rightVal  < THREASHOLD;
		
		// construct the bargraph information from the adc values
		shiftVal = 1 << 31;
		
		for (int i = 0; i < leftVal / 410 ; i++) {
			shiftVal = shiftVal	| (1<<i);
		}
		for (int i = 10; i < (middleVal / 410)+10 ; i++) {
			shiftVal = shiftVal	| (1<<i);
		}
		for (int i = 20; i < (rightVal / 410)+20 ; i++) {
			shiftVal = shiftVal	| (1<<i);
		}
		
		// add information if theashold is reached
		shiftVal |= (left<<9);
		shiftVal |= (middle<<19);
		shiftVal |= (right<<29);
		
		shiftreg_send(shiftVal);
	} else {
		
		
		//  blink the bargraphs to show if the robot is running
		if (timestamp == TIMEINTERVALL) {
			shiftreg_send(1<<9);
		} else if (timestamp == 2*TIMEINTERVALL) {
			shiftreg_send(1<<19);
		} else if (timestamp == 3*TIMEINTERVALL) {
			shiftreg_send(1<<29);
		} else if (timestamp == 4*TIMEINTERVALL) {
			shiftreg_send(1<<19);
			timestamp = 0;
		}
		timestamp++;
	}
	
	if (testmode) {
			printf("left = %u | middle = %u | right = %u \n", 
					leftVal, middleVal, rightVal);
	}
	
	if (currentlydriving) {
		if (middle) {
			lastleft = false;
			lastright = false;
			driveForeward();
			send_string("|\n",3);
		} else if (right) {
			lastleft = false;
			lastright = true;
			turnRight();
			send_string("/\n",3);
		} else if (left) {
			lastleft = true;
			lastright = false;
			turnLeft();
			send_string("\\\n",3);
		} else if (lastleft) {
			rotateLeft();
			send_string("<\n",3);
		} else if (lastright) {
			rotateRight();
			send_string(">\n",3);
		} else {
			driveForeward();
			send_string("|\n",3);
		}
		sleep_ms(10);
	}
	
	}
    
}

