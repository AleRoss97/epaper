/* -------------------------------------------------------------------------- */
/* NAME: Display Grid on InkyPhat Pimoroni                                    */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* -------------------------------------------------------------------------- */
/* DESCRIPTION                                                                */
/* This program does the following:                                           */
/* - Reads the prova.bmp image in the same folder of the executable           */
/* - For each pixel:                                                          */
/*   - Computes the luminance level for each RGB pixel                        */
/*   - Sets the pixel accordingly (BLACK, WHITE, YELLOW/RED)                  */
/* - Shows the image on the display                                           */
/* -------------------------------------------------------------------------- */

#include "epaper.h"
#include "cbmp.h"
#include <unistd.h>
#include <stdio.h>


static void delay_ms(uint32_t ms) {
	usleep(1000*ms);
}

uint32_t max(uint8_t v1,uint8_t v2,uint8_t v3) {
	uint32_t temp;

	if (v1 >= v2) temp = v1;
	else          temp = v2;

	if (temp >= v3) temp = temp;
	else            temp = v3;

	return temp;
}
uint32_t min(uint8_t v1,uint8_t v2,uint8_t v3) {
	uint32_t temp;

	if (v1 < v2) temp = v1;
	else          temp = v2;

	if (temp < v3) temp = temp;
	else            temp = v3;

	return temp;
}
int main(int argc, char const *argv[]) {

    unsigned int x, y;
    unsigned char r=0, g=0, b=0, l=0;

    printf("[MAIN] OPENING DISPLAY...\n");
	epaper* display = new epaper();

	/* Open the input image */
	printf("[MAIN] OPENING IMAGES...\n");
    BMP* bmp1 = bopen("prova.bmp");	

	printf("[MAIN] CHECKING IMAGES DIMENSIONS...\n");
    if (get_width(bmp1) != display->heigth) {
    	printf("Image WIDTH is not of the right size! %d %d\n", get_width(bmp1), display->width);
    	return 1;
    }
    if (get_height(bmp1) != display->width) {
    	printf("Image WIDTH is not of the right size! %d %d\n", get_height(bmp1), display->heigth);
    	return 2;
    }

    printf("[MAIN] SETTING BUFFER FOR FIRST IMAGE...\n");
    for (x = 0; x < display->heigth; x++) {
        for (y = 0; y < display->width; y++) {

        	/* Get pixel */
            get_pixel_rgb(bmp1, x, y, &r, &g, &b);

            /* Calculate luminance */
            l = (max(r,g,b)+min(r,g,b))/2;

            /* Set color based on luminance */
			if (l > 178) {
				display->buff[y][x] = WHITE;
			}
			else {
				if (l < 84) {
					display->buff[y][x] = BLACK;
				}
				else { 
					display->buff[y][x] = YELLOW;
				}
			}
			
        }
    }

    printf("[MAIN] SHOWING TO DISPLAY...\n");
	display->show(0);
    
    /* End application */
    bclose(bmp1);
	return 0;
}
