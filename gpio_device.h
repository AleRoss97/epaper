#ifndef _GPIO_DEVICE_
#define _GPIO_DEVICE_
/* -------------------------------------------------------------------------- */
/* NAME: GPIO Device H - Raspberry Pi Zero W                                  */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* SOC: BCM2835                                                               */
/* DOCUMENT REFERENCE: BCM2835-ARM-Peripherals.pdf                            */
/* -------------------------------------------------------------------------- */

#include <stdint.h>

/* ---------------------------------------------------------------------- */
/*                |------|------|          ||                             */
/*            3V3 |  (1) | (2)  | 5V       ||                             */
/*          GPIO2 |  (3) | (4)  | 5V       ||                             */
/*          GPIO3 |  (5) | (6)  | GND      ||                             */
/*          GPIO4 |  (7) | (8)  | GPIO14   ||                             */
/*            GND |  (9) | (10) | GPIO15   ||                             */
/*         GPIO17 | (11) | (12) | GPIO18   ||                             */
/*         GPIO27 | (13) | (14) | GND      ||                             */
/*         GPIO22 | (15) | (16) | GPIO23   ||                             */
/*            3V3 | (17) | (18) | GPIO24   ||                             */
/*         GPIO10 | (19) | (20) | GND      ||                             */
/*          GPIO9 | (21) | (22) | GPIO25   ||                             */
/*         GPIO11 | (23) | (24) | GPIO8    ||                             */
/*            GND | (25) | (26) | GPIO7    ||                             */
/*          GPIO0 | (27) | (28) | GPIO1    ||                             */
/*          GPIO5 | (29) | (30) | GND      ||                             */
/*          GPIO6 | (31) | (32) | GPIO12   ||                             */
/*         GPIO13 | (33) | (34) | GND      ||                             */
/*         GPIO19 | (35) | (36) | GPIO16   ||                             */
/*         GPIO26 | (37) | (38) | GPIO20   ||                             */
/*            GND | (39) | (40) | GPIO21   ||                             */
/*                |------|------|          ||                             */
/* ---------------------------------------------------------------------- */

class gpio_device {

	public:

		#define LOW 0
		#define HIGH 1

		typedef enum direction_e {
			INPUT = 0,
			OUTPUT = 1,
			ALTERNATE_0 = 2,
			ALTERNATE_1 = 3,
			ALTERNATE_2 = 4,
			ALTERNATE_3 = 5,
			ALTERNATE_4 = 6,
			ALTERNATE_5 = 7
		} direction_t;
		typedef enum pull_e {
			UP,
			DOWN,
			REMOVE
		} pull_t;

		/* ------------------------------ */
		/* Constructor                    */
		/* ------------------------------ */
		/* Maps the real memory in the    */
		/* virtual space for this         */
		/* proces.                        */
		/* Does nothing on the gpio port. */
		/* ------------------------------ */
		gpio_device();

		/* ------------------------------ */
		/* Set directions                 */
		/* ------------------------------ */
		/* For the pin given, you can set */
		/* the driection see              */
		/* (direction_t)                  */
		/* The PIN is numbered as the SOC */
		/* datasheet                      */
		/* ------------------------------ */
		void direction(uint32_t pin, direction_t dir);

		/* ------------------------------ */
		/* Set voltage level              */
		/* ------------------------------ */
		/* For the pin given, you can set */
		/* the voltage level              */
		/* (see uint8_t)                  */
		/* The PIN is numbered as the SOC */
		/* datasheet                      */
		/* ------------------------------ */
		void set(uint32_t pin, uint8_t state);

		/* ------------------------------ */
		/* Get voltage level              */
		/* ------------------------------ */
		/* For the pin given, you can get */
		/* voltage level                  */
		/* The PIN is numbered as the SOC */
		/* datasheet                      */
		/* ------------------------------ */
		uint8_t get(uint32_t pin);

		/* ------------------------------ */
		/* Manage PULL-UPs PULL-DOWNs     */
		/* ------------------------------ */
		/* For the pin given, you can set */
		/* or remove pull ups and pull    */
		/* downs                          */
		/* The PIN is numbered as the SOC */
		/* datasheet                      */
		/* ------------------------------ */
		void pull(uint32_t pin, pull_t type);

		/* ------------------------------ */
		/* Constructor                    */
		/* ------------------------------ */
		/* Unmaps the real memory in the  */
		/* virtual space for this         */
		/* proces.                        */
		/* Does nothing on the gpio port. */
		/* ------------------------------ */
		~gpio_device();

	private:

		int mem_fd;

		uint32_t* gpio_fsel[6];
		uint32_t* gpio_set[2];
		uint32_t* gpio_clr[2];
		uint32_t* gpio_lev[2];
		uint32_t* gpio_eds[2];
		uint32_t* gpio_aren[2];
		uint32_t* gpio_afen[2];
		uint32_t* gpio_ren[2];
		uint32_t* gpio_fen[2];
		uint32_t* gpio_hen[2];
		uint32_t* gpio_len[2];
		uint32_t* gpio_pud;
		uint32_t* gpio_pudclk[2];

};

#endif
