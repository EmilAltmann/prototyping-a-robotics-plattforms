#include "motors.h"

static int motor_offset;

/// int PWM and IO for driving the stepper motors
void initDrive(){
	
	// init pins for output
	gpio_init(LD);
	gpio_init(LS);
	gpio_init(RD);
	gpio_init(RS);
	
	gpio_set_dir(LD, GPIO_OUT);
    gpio_set_dir(LS, GPIO_OUT);
    gpio_set_dir(RD, GPIO_OUT);
    gpio_set_dir(RS, GPIO_OUT);
    
    // set drection to known levels
    gpio_put(LD, 1 );
	gpio_put(RD, 1 );
	
	// init PWM
	gpio_set_function(LS, GPIO_FUNC_PWM);
    gpio_set_function(RS, GPIO_FUNC_PWM);
    slice_L   = pwm_gpio_to_slice_num(LS);
	channel_L = pwm_gpio_to_channel(LS);
	slice_R   = pwm_gpio_to_slice_num(RS);
	channel_R = pwm_gpio_to_channel(RS);

	// for the left side
    pwm_set_clkdiv(slice_L, MAXSPEED);  /// Setting the divider to slow down the clock
    pwm_set_wrap(slice_L, WRAPVAL);      /// setting the Wrap time to limit max speed
	pwm_set_chan_level(slice_L, channel_L, WRAPVAL/2);
    pwm_set_enabled(slice_L, false);
    
    // for the right side
    pwm_set_clkdiv(slice_R, MAXSPEED);  /// Setting the divider to slow down the clock
    pwm_set_wrap(slice_R, WRAPVAL);      /// setting the Wrap time to limit max speed
    pwm_set_chan_level(slice_R, channel_R, WRAPVAL/2);
    pwm_set_enabled(slice_R, false);
}

// set speed between -100 and 100
void set_speed(uint8_t speed_l, uint8_t speed_r) {
	
	if (speed_l == 0) {
		pwm_set_enabled(slice_L, false);
	} else {
		pwm_set_enabled(slice_L, true);
	}
	
	if (speed_r == 0) {
		pwm_set_enabled(slice_R, false);
	} else {
		pwm_set_enabled(slice_R, true);
	}	
	
	pwm_set_clkdiv(slice_L, speed_l);
	pwm_set_clkdiv(slice_R, speed_r);
}

void driveForeward(){
	set_speed(HIGHSPEED,HIGHSPEED);
	gpio_put(LD, 1 );
	gpio_put(RD, 1 );
}

void driveBackwrd(){
	set_speed(HIGHSPEED,HIGHSPEED);
	gpio_put(LD, 0 );
	gpio_put(RD, 0 );
}

void rotateLeft(){
	set_speed(HIGHSPEED,HIGHSPEED);
	gpio_put(LD, 0 );
	gpio_put(RD, 1 );
}

void turnLeft() {
	set_speed(0,HIGHSPEED);
	gpio_put(LD, 0 );
	gpio_put(RD, 1 );
	}

void rotateRight(){
	set_speed(HIGHSPEED,HIGHSPEED);
	gpio_put(LD, 1 );
	gpio_put(RD, 0 );
}

void turnRight() {
	set_speed(HIGHSPEED,0);
	gpio_put(LD, 1 );
	gpio_put(RD, 0 );
	}


void stop(){
	set_speed(0,0);
}
