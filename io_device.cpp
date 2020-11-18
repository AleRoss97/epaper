#include "io_device.h"

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

#define SCALING_X_1 0
#define SCALING_X_2 1
#define SCALING_X_4 2
#define SCALING_X_8 3
#define SCALING_X_16 4
#define SCALING_X_32 5
#define SCALING_X_64 6

#define SAMP_TIME_320_US 0
#define SAMP_TIME_640_US 1
#define SAMP_TIME_1280_US 2
#define SAMP_TIME_2560_US 3

#define CYCLE_TIME_35_MS 0
#define CYCLE_TIME_70_MS 1
#define CYCLE_TIME_105_MS 2
#define CYCLE_TIME_140_MS 3

//#define LOG(...) printf("\033[0;35m[IO  ]"); printf(__VA_ARGS__); printf("\033[0m\n");
#define LOG(...)


io_device::io_device() {

	LOG("Opening device");
	i2c = new i2c_device("/dev/i2c-1", 0x2c);

	LOG("Setting default values for parameters");
	led_status = 0x00;
	risetime_status = RISE_FALL_250_MS;
	falltime_status = RISE_FALL_250_MS;
	minbright_status = MIN_DUTY_CYCLE_0_PERC;
	maxbright_status = MAX_DUTY_CYCLE_7_PERC;

	LOG("Setting default values for hardware");
	uint8_t averaging        = AVERAGING_8_SAMPLES;
	uint8_t gain             = GAIN_X1;
	uint8_t sensitivity      = SENSITIVITY_X32;
	uint8_t scaling          = SCALING_X_64;
	uint8_t interrupt_repeat = 0;
	uint8_t sample_time      = SAMP_TIME_1280_US;
	uint8_t cycle_time       = CYCLE_TIME_35_MS;
	uint8_t threshold_0      = 0x40;
	uint8_t threshold_1      = 0x40;
	uint8_t threshold_2      = 0x40;
	uint8_t threshold_3      = 0x40;
	uint8_t threshold_4      = 0x40;
	uint8_t threshold_5      = 0x40;

	LOG("Sending defaults to device");
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
	i2c->writeSingle(0x30, threshold_0);
	i2c->writeSingle(0x31, threshold_1);
	i2c->writeSingle(0x32, threshold_2);
	i2c->writeSingle(0x33, threshold_3);
	i2c->writeSingle(0x34, threshold_4);
	i2c->writeSingle(0x35, threshold_5);
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
	i2c->writeSingle(0x93, 0x00 | (maxbright_status << 4) | (minbright_status << 0));
	i2c->writeSingle(0x94, 0x00 | (risetime_status << 3) | (falltime_status << 0));
	i2c->writeSingle(0x95, 0x00);
	i2c->writeSingle(0x74, led_status);

}
io_device::~io_device() {

	delete i2c;

}

void io_device::setLed(uint8_t quale) {
	LOG("setLed %d", quale);
	led_status |= (1<<quale);
	i2c->writeSingle(0x74, led_status);
}
void io_device::clearLed(uint8_t quale) {
	LOG("clearLed %d", quale);
	led_status &= (~(1<<quale));
	i2c->writeSingle(0x74, led_status);
}
void io_device::setRiseTime(uint8_t time) {
	LOG("setRiseTime %d", time);
	risetime_status = time;
	i2c->writeSingle(0x94, 0x00 | (risetime_status << 3) | (falltime_status << 0));
}
void io_device::setFalling(uint8_t time) {
	LOG("setFalling %d", time);
	falltime_status = time;
	i2c->writeSingle(0x94, 0x00 | (risetime_status << 3) | (falltime_status << 0));
}
void io_device::setMaxBrightness(uint8_t time) {
	LOG("setMaxBrightness %d", time);
	maxbright_status = time;
	i2c->writeSingle(0x93, 0x00 | (maxbright_status << 4) | (minbright_status << 0));
}
void io_device::setMinBrightness(uint8_t time) {
	LOG("setMinBrightness %d", time);
	minbright_status = time;
	i2c->writeSingle(0x93, 0x00 | (maxbright_status << 4) | (minbright_status << 0));
}

bool io_device::getButton(uint8_t quale) {
	LOG("getButton %d", quale);

	uint8_t valore;

	valore = i2c->readSingle(0x03);
	uint8_t toreturn = (valore >> quale) & 0x1;

	return toreturn;
}

void io_device::clearButtons() {

	LOG("clearButtons");

	/* Clear detection */
	uint8_t valore;
	valore = i2c->readSingle(0x00);
	valore = valore & (~(0x01));
	i2c->writeSingle(0x00, valore);

}