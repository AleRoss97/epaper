
#include <stdio.h>
#include <stdlib.h>

#include "i2c_device.h"

#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <sys/ioctl.h> /* For ioctl() */

extern "C" {
	#include <linux/i2c-dev.h>
	#include <i2c/smbus.h>
}

#define LOG(...) printf("\033[0;35m[I2C ] "); printf(__VA_ARGS__); printf("\033[0m\n");

i2c_device::i2c_device(char* i2c_device, uint8_t i2c_addr) {

	LOG("Opening device");
	fp = open(i2c_device, O_RDWR);
	if (fp < 0) {
		LOG("Unable to open device in %s.", i2c_device);
		exit(1);
	}

	int status; 

	LOG("Setting the address");
	status = ioctl(fp, I2C_SLAVE, i2c_addr);
	if (status < 0) {
		LOG("Unable to set I2C addr %d.", i2c_addr);
		exit(1);
	}

}

i2c_device::~i2c_device() {
	LOG("Closing device");
	close(fp);
}


void i2c_device::writeSingle(uint8_t reg, uint8_t value) {

	int32_t status;

	status = i2c_smbus_write_word_data(fp, reg, value);
	if (status < 0) {
		LOG("Write failed at address %d with value.", reg, value);
	}

}
uint8_t i2c_device::readSingle(uint8_t reg) {

	int32_t status;

	status = i2c_smbus_read_word_data(fp, reg);
	if (status < 0) {
		LOG("Read failed at address %d.", reg);
	}

	return (uint8_t)(status&0x000000FF);

}