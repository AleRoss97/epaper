#ifndef _GRAPHIC_DEVICE_
#define _GRAPHIC_DEVICE_

#include <stdint.h>
#include "gpio_device.h"
#include "spi_device.h"

class graphic_device {

	public:
		graphic_device();
		~graphic_device();

		static const uint32_t width = 128;
		static const uint32_t height = 64;

		uint8_t buff[width][height];

		void clean(uint8_t value);
		void contrast(uint8_t value);
		void show();


	private:
		gpio_device *gpio;
		spi_device  *spi;

		const uint32_t pin_cs = 8;
		const uint32_t pin_rst = 5;
		const uint32_t pin_dc = 6;

		void send_command(uint8_t cmd);
		void send_data(uint8_t data);

		/* Packed array */
		static const uint32_t packed_size = (width*height)/8;
		uint8_t packed[packed_size];
		/* To pack the array */
		void pack_buffer(void);

};

#endif
