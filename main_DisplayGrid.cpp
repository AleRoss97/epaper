/* -------------------------------------------------------------------------- */
/* NAME: Display Grid on InkyPhat Pimoroni                                    */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* -------------------------------------------------------------------------- */
/* DESCRIPTION                                                                */
/* This program does the following:                                           */
/* - Writes the pixels in the buffer to create a grid                         */
/* - Shows the image on the display                                           */
/* -------------------------------------------------------------------------- */

#include "epaper.h"
#include <unistd.h>
#include <stdio.h>


static void delay_ms(uint32_t ms) {
	usleep(1000*ms);
}

int main(int argc, char const *argv[]) {
	
	uint32_t x,y;

	epaper* display = new epaper();
	display->clear(WHITE);

	for (y=0; y<136; y++) {
		display->buff[y][250/2] = BLACK;
	}
	for (y=0; y<136; y+=10) {
		if (y == 10) {
			display->buff[y][250/2-4] = YELLOW;
			display->buff[y][250/2-5] = YELLOW;
			display->buff[y][250/2-6] = YELLOW;
		}
		display->buff[y][250/2-3] = YELLOW;
		display->buff[y][250/2-2] = YELLOW;
		display->buff[y][250/2-1] = YELLOW;
		display->buff[y][250/2  ]   = YELLOW;
		display->buff[y][250/2+1] = YELLOW;
		display->buff[y][250/2+2] = YELLOW;
		display->buff[y][250/2+3] = YELLOW;
		if (y == 10) {
			display->buff[y][250/2+4] = YELLOW;
			display->buff[y][250/2+5] = YELLOW;
			display->buff[y][250/2+6] = YELLOW;
		}
	}

	for (x=0; x<250; x++) {
		display->buff[136/2][x] = BLACK;
	}
	for (x=0; x<250; x+=10) {
		if (x == 0) {
			display->buff[136/2-6][x] = YELLOW;
			display->buff[136/2-5][x] = YELLOW;
			display->buff[136/2-4][x] = YELLOW;			
		}
		display->buff[136/2-3][x] = YELLOW;
		display->buff[136/2-2][x] = YELLOW;
		display->buff[136/2-1][x] = YELLOW;
		display->buff[136/2  ][x]   = YELLOW;
		display->buff[136/2+3][x] = YELLOW;
		display->buff[136/2+2][x] = YELLOW;
		display->buff[136/2+1][x] = YELLOW;
		if (x == 0) {
			display->buff[136/2+6][x] = YELLOW;
			display->buff[136/2+5][x] = YELLOW;
			display->buff[136/2+4][x] = YELLOW;			
		}
	}
	
	display->show(0);

	return 0;
}
