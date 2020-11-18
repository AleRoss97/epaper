#ifndef _BACKLIGHT_DEVICE_
#define _BACKLIGHT_DEVICE_

#include <stdint.h>
#include "i2c_device.h"

class backlight_device
{
	public:
		backlight_device();
		~backlight_device();
		
		void setColor(uint8_t r, uint8_t g, uint8_t b);

	private:
		i2c_device *i2c;
};

#endif
