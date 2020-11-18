#include <stdint.h>

#include "backlight_device.h"

int main(int argc, char const *argv[]) {

	uint32_t i;

	backlight_device *backlight = new backlight_device();

	for (i = 0;    i <  252; i+=4) backlight->setColor(i,0,0);
	for (i = 252; i != 0;    i-=4) backlight->setColor(i,0,0);
	backlight->setColor(0,0,0);

	for (i = 0;    i <  252; i+=4) backlight->setColor(0,i,0);
	for (i = 252; i != 0;    i-=4) backlight->setColor(0,i,0);
	backlight->setColor(0,0,0);

	for (i = 0;    i <  252; i+=4) backlight->setColor(0,0,i);
	for (i = 252; i != 0;    i-=4) backlight->setColor(0,0,i);
	backlight->setColor(0,0,0);

	for (i = 0;    i <  252; i+=4) backlight->setColor(i,i,i);
	for (i = 252; i != 0;    i-=4) backlight->setColor(i,i,i);
	backlight->setColor(0,0,0);

	/* code */
	return 0;
}
