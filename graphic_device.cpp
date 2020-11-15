#include "graphic_device.h"
#include <unistd.h>
#include <stdio.h>

#define LOG(...) printf("\033[0;31m[GRAP]"); printf(__VA_ARGS__); printf("\033[0m\n");

/* ---------------------------[ Display commands ]--------------------------- */

static const uint8_t ST7567_BIAS_1_7       = 0xa3;
static const uint8_t ST7567_SEG_DIR_NORMAL = 0xa0;
static const uint8_t ST7567_SETCOMNORMAL   = 0xc0;
static const uint8_t ST7567_SETCOMREVERSE  = 0xc8;
static const uint8_t ST7567_DISPNORMAL     = 0xa6;
static const uint8_t ST7567_SETSTARTLINE   = 0x40;
static const uint8_t ST7567_POWERCTRL      = 0x2f;
static const uint8_t ST7567_REG_RATIO      = 0x20;
static const uint8_t ST7567_DISPON         = 0xaf;
static const uint8_t ST7567_SETCONTRAST    = 0x81;
static const uint8_t ST7567_SETPAGESTART   = 0xb0;
static const uint8_t ST7567_SETCOLL        = 0x00;
static const uint8_t ST7567_SETCOLH        = 0x10;
static const uint8_t ST7567_EXIT_RMWMODE   = 0xee;

static const uint8_t ST7567_PAGESIZE       = 128;

/* -------------------------------[ Helpers ]-------------------------------- */

static void sleepms(uint32_t ms) {
	LOG("Sleeping %d ms", ms);
	usleep(1000*ms);
}

/* -------------------------------[ Publics ]-------------------------------- */

graphic_device::graphic_device() {

	LOG("Setting buffers to 0");
	clean(0);

	LOG("Opening GPIO device");
	gpio = new gpio_device();
	LOG("Opening SPI device");
	spi = new spi_device("/dev/spidev0.0", SPI_MODE_0, 8, 1000000);

	LOG("Setting GPIO directions");
	gpio->direction(pin_rst, gpio->OUTPUT);
	gpio->direction(pin_dc, gpio->OUTPUT);

	LOG("Resetting display");
	gpio->set(pin_rst, LOW);
    sleepms(100);
    gpio->set(pin_rst, HIGH);
    sleepms(100);

    LOG("Bias 1/7 (0xA2 = Bias 1/9)");
    send_command(ST7567_BIAS_1_7);
    LOG("Column address 0 is mapped to SEG0");
    send_command(ST7567_SEG_DIR_NORMAL);
    LOG("Reverse COM - vertical flip");
    send_command(ST7567_SETCOMREVERSE);
    LOG("Inverse display (0xA6 normal)");
    send_command(ST7567_DISPNORMAL);
    LOG("Start at line 0");
    send_command(ST7567_SETSTARTLINE | 0);
    LOG("Control built-in power circuit");
    send_command(ST7567_POWERCTRL);
    send_command(ST7567_REG_RATIO | 3);
    LOG("Display ON in normal mode");
    send_command(ST7567_DISPON);
    LOG("Set contrast");
    send_command(ST7567_SETCONTRAST);
    LOG("Contrast value");
    send_command(58);

}

graphic_device::~graphic_device() {

	delete gpio;
	delete spi;

}

void graphic_device::clean(uint8_t val) {

	uint32_t x,y;
	for (x=0; x<width; x++) {
		for (y=0; y<height; y++) {
			buff[x][y] = val;
		}
	}

}

void graphic_device::contrast(uint8_t value) {

	if (value < 64) {
	    LOG("Setting contrast to %d", value);
		send_command(ST7567_SETCONTRAST);
		send_command(value);
	}
	else {
		LOG("[WARNING] CALLED CONTRAST WITH %d, SHOULD BE LESS THEN 64.", value);
	}


}

void graphic_device::pack_buffer(void) {

	uint32_t x,y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
	        uint32_t offset = ((y / 8) * width) + x;
	        uint32_t bit = y % 8;
	        packed[offset] &= ~(1 << bit);
	        packed[offset] |= (buff[x][y] & 1) << bit;
		}
	}
}

void graphic_device::show(void) {

	/* Pack buff in packed */
    LOG("Packing buffer");
	pack_buffer();

	/* Send packed to the display */
    LOG("Sending packed buffer");
	uint8_t page;
	uint32_t index;
	for (page = 0; page < 8; page++) {

		/* Set current page */
		send_command(ST7567_SETPAGESTART | page);
		/* Reset column counter */
		send_command(ST7567_SETCOLL);
		send_command(ST7567_SETCOLH);

		/* Send current page from buffer */
		for (index = 0; index < ST7567_PAGESIZE; index++) {
			send_data(packed[page*ST7567_PAGESIZE+index]);
		}


	}

    LOG("Exiting raw mode");
	send_command(ST7567_EXIT_RMWMODE);

}

/* -------------------------------[ Privates ]------------------------------- */

void graphic_device::send_command(uint8_t cmd) {

    gpio->set(pin_dc, LOW);
    spi->writeSingle(cmd);

}
void graphic_device::send_data(uint8_t data) {

    gpio->set(pin_dc, HIGH);
    spi->writeSingle(data);

}
