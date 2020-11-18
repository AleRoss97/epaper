#include <stdio.h>
#include "graphic_device.h"
#include <unistd.h>

graphic_device *disp;

static void sleepms(uint32_t ms) {
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

	printf("[MAIN] Setting contrast\n");
	disp->contrast(38);

	printf("[MAIN] Displaying line\n");

	while (1) {
		uint32_t x,y;
		for (x = 0; x < disp->height-20; x+=10) {
			disp->clean(0);
			psquare(x,x,20);
			disp->show();
			sleepms(100);
		}
	}

	printf("[MAIN] Exiting...\n");
	return 0;
}
