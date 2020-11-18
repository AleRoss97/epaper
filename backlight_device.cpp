
#include "backlight_device.h"
#include <stdio.h>
#include <unistd.h>

#define LOG(...) printf("\033[0;37m[I2C ] "); printf(__VA_ARGS__); printf("\033[0m\n");

backlight_device::backlight_device() {

	LOG("Opening device");
	i2c = new i2c_device("/dev/i2c-1", 0x54);

	LOG("Waking up controller");
	i2c->writeSingle(0x00, 0x01); /* wake up */

	printf("Send full off\n");
	i2c->writeSingle(0x01, 0x00); /* ch 0 brightness */
	i2c->writeSingle(0x02, 0x00); /* ch 1 brightness */
	i2c->writeSingle(0x03, 0x00); /* ch 2 brightness */

	i2c->writeSingle(0x04, 0x00); /* ch 3 brightness */
	i2c->writeSingle(0x05, 0x00); /* ch 4 brightness */
	i2c->writeSingle(0x06, 0x00); /* ch 5 brightness */
	
	i2c->writeSingle(0x07, 0x00); /* ch 6 brightness */
	i2c->writeSingle(0x08, 0x00); /* ch 7 brightness */
	i2c->writeSingle(0x09, 0x00); /* ch 8 brightness */
	
	i2c->writeSingle(0x0A, 0x00); /* ch 9 brightness */
	i2c->writeSingle(0x0B, 0x00); /* ch 10 brightness */
	i2c->writeSingle(0x0C, 0x00); /* ch 11 brightness */
	
	i2c->writeSingle(0x0D, 0x00); /* ch 12 brightness */
	i2c->writeSingle(0x0E, 0x00); /* ch 13 brightness */
	i2c->writeSingle(0x0F, 0x00); /* ch 14 brightness */
	
	i2c->writeSingle(0x10, 0x00); /* ch 15 brightness */
	i2c->writeSingle(0x11, 0x00); /* ch 16 brightness */
	i2c->writeSingle(0x12, 0x00); /* ch 17 brightness */

	i2c->writeSingle(0x13, 0x3F); /* led on */
	i2c->writeSingle(0x14, 0x3F); /* len on */
	i2c->writeSingle(0x15, 0x3F); /* led on */
}
backlight_device::~backlight_device() {
	delete i2c;
}

void backlight_device::setColor(uint8_t r, uint8_t g, uint8_t b) {

	i2c->writeSingle(0x01, b); /* led 3 b */
	i2c->writeSingle(0x02, g); /* led 3 g */
	i2c->writeSingle(0x03, r); /* led 3 r */

	i2c->writeSingle(0x04, b); /* led 2 b */
	i2c->writeSingle(0x05, g); /* led 2 g */
	i2c->writeSingle(0x06, r); /* led 2 r */
	
	i2c->writeSingle(0x07, b); /* led 1 b */
	i2c->writeSingle(0x08, g); /* led 1 g */
	i2c->writeSingle(0x09, r); /* led 1 r */
	
	i2c->writeSingle(0x0A, b); /* led 6 b */
	i2c->writeSingle(0x0B, g); /* led 6 g */
	i2c->writeSingle(0x0C, r); /* led 6 r */
	
	i2c->writeSingle(0x0D, b); /* led 5 b */
	i2c->writeSingle(0x0E, g); /* led 5 g */
	i2c->writeSingle(0x0F, r); /* led 5 r */
	
	i2c->writeSingle(0x10, b); /* led 4 b */
	i2c->writeSingle(0x11, g); /* led 4 g */
	i2c->writeSingle(0x12, r); /* led 4 r */

	i2c->writeSingle(0x13, 0x3F); /* led on */
	i2c->writeSingle(0x14, 0x3F); /* len on */
	i2c->writeSingle(0x15, 0x3F); /* led on */

}
