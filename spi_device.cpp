/* -------------------------------------------------------------------------- */
/* NAME: SPI Device CPP - Raspberry Pi Zero W                                 */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* SOC: BCM2835                                                               */
/* DOCUMENT REFERENCE: BCM2835-ARM-Peripherals.pdf                            */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "spi_device.h"
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <sys/ioctl.h> /* For ioctl() */

#define LOG(...) printf("\033[0;33m[GRAP]"); printf(__VA_ARGS__); printf("\033[0m\n");

static void assert(uint8_t cond, char* desc) {

	if (!cond) {
		fprintf(stderr, "\n\n[ERROR] %s\n\n\n", desc);
		exit(1);
	}

}

spi_device::spi_device(
	char* spi_device,
	uint8_t spi_mode = SPI_MODE_0,
	uint8_t spi_bitn = 8,
	uint32_t spi_speed = 1000000
) {

	int status_value0 = -1;
	int status_value1 = -1;
	int status_value2 = -1;
	int status_value3 = -1;
	int status_value4 = -1;
	int status_value5 = -1;

	LOG("Open device.");

	fd = open(spi_device, O_RDWR);
	assert(fd >= 0, "Unable to open the device file.");

	LOG("Set parameters for device.");

	status_value0 = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);
	status_value1 = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);
	status_value2 = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitn);
	status_value3 = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitn);
	status_value4 = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	status_value5 = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);

	assert(status_value0 >= 0, "Unable to set SPI_IOC_WR_MODE.");
	assert(status_value1 >= 0, "Unable to set SPI_IOC_RD_MODE.");
	assert(status_value2 >= 0, "Unable to set SPI_IOC_WR_BITS_PER_WORD.");
	assert(status_value3 >= 0, "Unable to set SPI_IOC_RD_BITS_PER_WORD.");
	assert(status_value4 >= 0, "Unable to set SPI_IOC_WR_MAX_SPEED_HZ.");
	assert(status_value5 >= 0, "Unable to set SPI_IOC_RD_MAX_SPEED_HZ.");

	LOG("Save value.");

	dev_mode = spi_mode;
	dev_bitn = spi_bitn;
	dev_speed = spi_speed;

}

spi_device::~spi_device() {

	LOG("Close the device.");

	close(fd);
	assert(fd >= 0, "Unable to close the device file.");

}

void spi_device::transferBuf(uint8_t *rdata, uint8_t *wdata, uint32_t size) {

	int status;
	struct spi_ioc_transfer spi;

	spi.tx_buf = (uint64_t)wdata;
	spi.rx_buf = (uint64_t)rdata;
	spi.len = size;

	spi.speed_hz = dev_speed;
	spi.bits_per_word = dev_bitn;

	spi.delay_usecs = 0; /* Delay between words */
	spi.cs_change = 1;   /* 1: CS stays low between words, 0 : CS goes high between words */

	status = ioctl(fd, SPI_IOC_MESSAGE(1), &spi);
	assert(status >= 0, "Transfer failed.");

}
void spi_device::writeBuf(uint8_t *bufdata, uint32_t bufsize) {

	int status;
	status = write(fd, bufdata, bufsize);
	assert(status >= 0, "Transfer failed.");

}
void spi_device::readBuf(uint8_t *bufdata, uint32_t bufsize) {

	int status;
	status = read(fd, bufdata, bufsize);
	assert(status >= 0, "Transfer failed.");

}

void spi_device::writeSingle(uint8_t to_transfer) {

	uint32_t len = 1;
	writeBuf(&to_transfer, len);

}
uint8_t spi_device::readSingle(void) {

	uint8_t to_ret;
	uint32_t len = 1;
	readBuf(&to_ret, len);
	return to_ret;

}
uint8_t spi_device::transferSingle(uint8_t to_transfer) {

	uint8_t to_ret;
	uint32_t len = 1;
	transferBuf(&to_ret, &to_transfer, len);
	return to_ret;

}
