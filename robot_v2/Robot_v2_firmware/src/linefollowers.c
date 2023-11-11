#include "linefollowers.h"

void init_linefollowers() {
	adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
	adc_gpio_init(28);
	}
	
uint16_t sample_linefollower(uint8_t theOne) {
	
	uint16_t adcval = 0;
	adc_select_input(theOne);
	
	for (int i = 0; i < adc_SAMPLE_SIZE; i++) {
		adcval += adc_read();
	}
	
	return adcval / adc_SAMPLE_SIZE;
}
