#ifndef _IO_DEVICE_
#define _IO_DEVICE_

#include <stdint.h>
#include "i2c_device.h"

class io_device {
	public:

		io_device();
		~io_device();

		void setLed(uint8_t quale);
		void clearLed(uint8_t quale);
		void setRiseTime(uint8_t time);
		void setFalling(uint8_t time);
		void setMaxBrightness(uint8_t time);
		void setMinBrightness(uint8_t time);

		bool getButton(uint8_t quale);
		void clearButtons(void);

	private:
		
		uint8_t led_status;
		uint8_t risetime_status;
		uint8_t falltime_status;
		uint8_t maxbright_status;
		uint8_t minbright_status;

		i2c_device *i2c;

};


#endif
