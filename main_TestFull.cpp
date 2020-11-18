
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "io_device.h"
#include "backlight_device.h"
#include "graphic_device.h"

graphic_device *disp;
backlight_device *backlight;
io_device *io;

static void delayms(uint32_t ms) {
	printf("[MAIN] Sleeping %d ms\n", ms);
	usleep(1000*ms);
}

void psquare(uint32_t startx, uint32_t starty, uint32_t len) {

	printf("[MAIN] Square at %d,%d of size %d\n", startx, starty, len);

	uint32_t x,y;
	for (x = startx; x < (startx+len); x++) {
		for (y = starty; y < (starty+len); y++) {
			disp->buff[x][y] = 1;
		}
	}
}

int main(int argc, char const *argv[]) {

	printf("[MAIN] Opening display\n");
	disp = new graphic_device();
	io = new io_device();
	backlight = new backlight_device();

	printf("[MAIN] Setting contrast\n");
	disp->contrast(38);

	printf("[MAIN] Entering main loop\n");
	uint32_t square_x = 0;
	uint8_t led = 0;
	uint8_t bright = 0;
	uint8_t col = 0;
	while (1) {

		/* Moving square */
		disp->clean(0);
		psquare(square_x,square_x,20);
		disp->show();
		square_x += 10;
		if (square_x > disp->height) square_x = 0;

		/* Light up led */
		io->setLed(led);
		led++;
		if (led >= 6) {
			led = 0;
			io->clearLed(0);
			io->clearLed(1);
			io->clearLed(2);
			io->clearLed(3);
			io->clearLed(4);
			io->clearLed(5);
		}

		/* Brightness */
		if (col/26 == 0) {
			backlight->setColor(bright,0,0);
		}
		else if (col/26 == 1) {
			backlight->setColor(0,bright,0);
		}
		else if (col/26 == 2) {
			backlight->setColor(0,0,bright);
		}
		else {
			backlight->setColor(0,0,0);
			col = 0;
		}
		
		bright+=10;
		col++;

		delayms(50);

	}

	printf("[MAIN] Exiting...\n");
	return 0;
}
