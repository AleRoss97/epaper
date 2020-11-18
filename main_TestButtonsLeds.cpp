#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "io_device.h"

static void delayms(uint32_t ms) {
	usleep(1000*ms);
}

int main(int argc, char const *argv[]) {

	printf("[MAIN] Opening IO device\n");
	io_device *io = new io_device();
	
	printf("[MAIN] Entering main loop\n");
	uint32_t i;
	while (1) {

		for (i = 0; i < 6; i++) {
			if (io->getButton(i)) printf("1");
			else printf("0");
		}

		io->clearButtons();
		printf("\n");

	}

	printf("[MAIN] Exiting\n");
	return 0;
}
