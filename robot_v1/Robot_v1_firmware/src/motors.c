#include "motors.h"

static int motor_offset;

void initDrive(){
	
	// add PIO programm to the Statemashines memory
	//~ printf("can put code for motor: %u",pio_can_add_program(pio0, &motors_program));
    motor_offset = pio_add_program(pio0, &motors_program);
    //~ printf(" with offset: %u\n",motor_offset);
    
    // get and configur the config
    pio_sm_config conf0 = motors_program_get_default_config(motor_offset);
	pio_sm_config conf1 = motors_program_get_default_config(motor_offset);

    // Map the state machine's SET pin group
    sm_config_set_set_pins(&conf0, 2, 4);
    sm_config_set_set_pins(&conf1, 6, 4);
    
    //hand gpio 2–9 over to pio0
    for (int i=2 ; i<=9 ; i++) {
		//~ gpio_set_function(i, GPIO_FUNC_PIO0);
		pio_gpio_init(pio0,i);
	}

	// init the Statemashines for the both motors
	pio_sm_init(pio0, sm0, motor_offset, &conf0);
    pio_sm_init(pio0, sm1, motor_offset, &conf1);
    
    // set set-pins for the motors
    pio_sm_set_consecutive_pindirs(pio0,sm0,2,4,true);
    pio_sm_set_consecutive_pindirs(pio0,sm1,6,4,true);
}

void driveForeward(){
	drive(100,-100);
}

void driveBackwrd(){
	drive(-100,100);
}

void turnLeft(){
	drive(0,-100);
}

void rotateLeft(){
	drive(-100,-100);
}

void turnRight(){
	drive(100,0);
}

void rotateRight(){
	drive(100,100);
}
	

void drive(int left, int right) {
	
	if(left < -100) {
		left = -100;
	} else if(left > 100) {
		left = 100;
	}
	
	if(right < -100) {
		right = -100;
	} else if(right > 100) {
		right = 100;
	}
	
	if(left == 0) {
		
		pio_sm_set_enabled(pio0, sm0, false);
		
	} else {
		if(left < 0){
			//turns backward
			left = -left;
			//jump to backward loop 0x000a
			pio_sm_exec(pio0, sm0, pio_encode_jmp(motor_offset + 9));
			//~ pio_sm_exec(pio0, sm0, 0x000a);
		} else {
			//turn forward
			// jump to forward loop 0x0001
			pio_sm_exec(pio0, sm0, pio_encode_jmp(motor_offset + 0));
			//~ pio_sm_exec(pio0, sm0, 0x0001);
		}
		//for 28ByJ-48: slow 65535 fast 5000
		pio_sm_set_clkdiv_int_frac(pio0 ,sm0, ((60000-600*left)+MINDIV), 0);
		pio_sm_set_enabled(pio0, sm0, true);
	}
	
	if(right == 0) {
		
		pio_sm_set_enabled(pio0, sm1, false);
		
	} else {
		if(right < 0){
			//turns backward
			right = -right;
			//jump to backward loop 0x000a
			pio_sm_exec(pio0, sm1, pio_encode_jmp(motor_offset+0xa));
		} else {
			//turn forward
			// jump to forward loop 0x0001
			pio_sm_exec(pio0, sm1, pio_encode_jmp(motor_offset));
		}
		//for 28ByJ-48: slow 65535 fast 5000
		pio_sm_set_clkdiv_int_frac(pio0 ,sm1, ((60000-600*right)+MINDIV), 0);
		pio_sm_set_enabled(pio0, sm1, true);
	}
	
}
