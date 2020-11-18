#ifndef _EPAPER_DEVICE_
#define _EPAPER_DEVICE_
/* -------------------------------------------------------------------------- */
/* NAME: Epaper Display CPP - Inky phat Pimoroni                              */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* SOC: SSD1608                                                               */
/* DOCUMENT reference: SSD1608_1.2.PDF                                        */
/* -------------------------------------------------------------------------- */

#include <stdint.h>

#include "spi_device.h"
#include "gpio_device.h"

class epaper {

	public:

		/* COLORS */
		#define WHITE  0
		#define BLACK  1
		#define YELLOW 2

		epaper();
		~epaper();
		
		void show(uint8_t wait);
		void clear(uint8_t color);

		static const uint32_t width = 136;
		static const uint32_t heigth = 250;

		/* Must contains only COLORS */
		uint8_t buff[width][heigth];

		static const uint32_t mem_len = (width*heigth)/8;
		uint8_t mem_data[2][mem_len];

	private:

		spi_device *spi;
		gpio_device *gpio;

		const uint32_t cols = 136;
		const uint32_t rows = 250;

		void send_both(uint8_t cmd, uint8_t data);
		void send_command(uint8_t cmd);
		void send_data(uint8_t data);
		uint8_t recv_data(void);

		void buff_to_mem(void);

		void wait_while_busy();

};

#endif
