
#include "i2c_device.h"
#include <unistd.h>
#include <stdio.h>

#define RISE_FALL_0_MS     0x00
#define RISE_FALL_250_MS   0x01
#define RISE_FALL_500_MS   0x02
#define RISE_FALL_750_MS   0x03
#define RISE_FALL_1000_MS  0x04
#define RISE_FALL_1250_MS  0x05
#define RISE_FALL_1500_MS  0x06
#define RISE_FALL_25000_MS 0x07

#define MAX_DUTY_CYCLE_7_PERC  0x00  
#define MAX_DUTY_CYCLE_9_PERC  0x01  
#define MAX_DUTY_CYCLE_11_PERC 0x02  
#define MAX_DUTY_CYCLE_14_PERC 0x03  
#define MAX_DUTY_CYCLE_17_PERC 0x04  
#define MAX_DUTY_CYCLE_20_PERC 0x05  
#define MAX_DUTY_CYCLE_23_PERC 0x06  
#define MAX_DUTY_CYCLE_26_PERC 0x07  
#define MAX_DUTY_CYCLE_30_PERC 0x08  
#define MAX_DUTY_CYCLE_35_PERC 0x09  
#define MAX_DUTY_CYCLE_40_PERC 0x0A  

#define MIN_DUTY_CYCLE_0_PERC  0x00 
#define MIN_DUTY_CYCLE_7_PERC  0x01 
#define MIN_DUTY_CYCLE_9_PERC  0x02 
#define MIN_DUTY_CYCLE_11_PERC 0x03 
#define MIN_DUTY_CYCLE_14_PERC 0x04 
#define MIN_DUTY_CYCLE_17_PERC 0x05 
#define MIN_DUTY_CYCLE_20_PERC 0x06 
#define MIN_DUTY_CYCLE_23_PERC 0x07 
#define MIN_DUTY_CYCLE_26_PERC 0x08 
#define MIN_DUTY_CYCLE_30_PERC 0x09 
#define MIN_DUTY_CYCLE_35_PERC 0x0A 

#define AVERAGING_1_SAMPLES 0
#define AVERAGING_2_SAMPLES 1
#define AVERAGING_4_SAMPLES 2
#define AVERAGING_8_SAMPLES 3
#define AVERAGING_16_SAMPLES 4
#define AVERAGING_32_SAMPLES 5
#define AVERAGING_64_SAMPLES 6
#define AVERAGING_128_SAMPLES 7

#define GAIN_X1 0
#define GAIN_X2 1
#define GAIN_X4 2
#define GAIN_X8 3

/* Most sensitivity */
#define SENSITIVITY_X128 0
#define SENSITIVITY_X64 1
#define SENSITIVITY_X32 2
#define SENSITIVITY_X16 3
#define SENSITIVITY_X8 4
#define SENSITIVITY_X4 5
#define SENSITIVITY_X2 6
#define SENSITIVITY_X1 7
/* Least sensitity */

i2c_device *i2c;

static void delayms(uint32_t ms) {
	usleep(1000*ms);
}

int main(int argc, char const *argv[]) {

	/* Open I2C */
	printf("[MAIN] Opening device.\n");
	i2c = new i2c_device("/dev/i2c-1", 0x2c);
	
	/* Alloca */
	uint8_t threshold[8];
	uint8_t gain;        //table 6-3
	uint8_t sensitivity; //table 6-8
	uint8_t scaling;
	uint8_t interrupt_repeat;
	uint8_t averaging;
	uint8_t sample_time;
	uint8_t cycle_time;
	uint8_t risetime;
	uint8_t falltime;
	uint8_t maxbright;
	uint8_t minbright;

	/* Scrivi */
	printf("[MAIN] Setting values for buttons.\n");
	averaging        = AVERAGING_8_SAMPLES;
	gain             = GAIN_X1;
	sensitivity      = SENSITIVITY_X32;
	scaling          = 0;
	interrupt_repeat = 0;
	sample_time      = 0;
	cycle_time       = 0;
	threshold[0]     = 0;
	threshold[1]     = 0;
	threshold[2]     = 0;
	threshold[3]     = 0;
	threshold[4]     = 0;
	threshold[5]     = 0;
	threshold[6]     = 0;
	threshold[7]     = 0;
	printf("[MAIN] Setting values for leds.\n");
	risetime         = RISE_FALL_500_MS;
	falltime         = RISE_FALL_500_MS;
	minbright        = MIN_DUTY_CYCLE_0_PERC;
	maxbright        = MAX_DUTY_CYCLE_7_PERC;


	printf("[MAIN] Configuring device.\n");

	/* Invia */
	i2c->writeSingle(0x00, 0x00 | (gain << 6));
	i2c->writeSingle(0x1F, 0x00 | (sensitivity << 4) | (scaling << 0));
	i2c->writeSingle(0x20, 0x70);
	i2c->writeSingle(0x44, 0x30);
	i2c->writeSingle(0x21, 0x3F);
	i2c->writeSingle(0x22, 0xF0 | interrupt_repeat);
	i2c->writeSingle(0x23, 0x00);
	i2c->writeSingle(0x24, 0x00 | (averaging < 4) | (sample_time << 2) | (cycle_time << 0));
	i2c->writeSingle(0x26, 0x00);
	i2c->writeSingle(0x27, 0x3F);
	i2c->writeSingle(0x28, 0x00);
	i2c->writeSingle(0x2A, 0x0C);
	i2c->writeSingle(0x2B, 0x00);
	i2c->writeSingle(0x2D, 0x00);
	i2c->writeSingle(0x2F, 0x0A);
	i2c->writeSingle(0x30, threshold[0]);
	i2c->writeSingle(0x31, threshold[1]);
	i2c->writeSingle(0x32, threshold[2]);
	i2c->writeSingle(0x33, threshold[3]);
	i2c->writeSingle(0x34, threshold[4]);
	i2c->writeSingle(0x35, threshold[5]);
	i2c->writeSingle(0x38, 0x03);
	i2c->writeSingle(0x40, 0x00);
	i2c->writeSingle(0x41, 0x00);
	i2c->writeSingle(0x42, 0x00);
	i2c->writeSingle(0x43, 0x00);
	i2c->writeSingle(0x71, 0x00);
	i2c->writeSingle(0x72, 0x00);
	i2c->writeSingle(0x73, 0x00);
	i2c->writeSingle(0x77, 0x00);
	i2c->writeSingle(0x79, 0x3F);
	i2c->writeSingle(0x81, 0x00);
	i2c->writeSingle(0x82, 0x00);
	i2c->writeSingle(0x84, 0x00);
	i2c->writeSingle(0x85, 0x00);
	i2c->writeSingle(0x86, 0x00);
	i2c->writeSingle(0x88, 0x00);
	i2c->writeSingle(0x90, 0x00);
	i2c->writeSingle(0x91, 0x00);
	i2c->writeSingle(0x92, 0x00);
	i2c->writeSingle(0x93, 0x00 | (maxbright << 4) | (minbright << 0));
	i2c->writeSingle(0x94, 0x00 | (risetime << 3) | (falltime << 0));
	i2c->writeSingle(0x95, 0x00);

	uint8_t valore;
	valore = i2c->readSingle(0xFD);
	printf("[MAIN] Product id={%02X}.\n", valore);
	valore = i2c->readSingle(0xFE);
	printf("[MAIN] Manufacturer id={%02X}.\n", valore);
	valore = i2c->readSingle(0xFF);
	printf("[MAIN] Revision id={%02X}.\n", valore);

	printf("[MAIN] Main loop.\n");
	uint8_t i = 0;
	while (1) {

		valore = i2c->readSingle(0x03);
		printf("[MAIN] Inputs: {%02X}.\n", valore);

		printf("[MAIN] Setting all HIGH.\n");
		i2c->writeSingle(0x74, 0xFF);
		delayms(250);
		printf("[MAIN] Setting all LOW.\n");
		i2c->writeSingle(0x74, 0x00);
		delayms(250);

		i++;
		
		if (i == 6) { i = 0; }
	}

	return 0;
}
