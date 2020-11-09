/* -------------------------------------------------------------------------- */
/* NAME: SPI Device H - Raspberry Pi Zero W                                   */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* SOC: BCM2835                                                               */
/* DOCUMENT REFERENCE: BCM2835-ARM-Peripherals.pdf                            */
/* -------------------------------------------------------------------------- */

#include <stdint.h>
#include <linux/spi/spidev.h>

/*

Handles 

REFERNECE DOCUMENTATION
-----------------------

https://www.kernel.org/doc/Documentation/spi/spidev

SPI MODES
---------

SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge

SPI DEVICES
-----------

/dev/spidev0.0
/dev/spidev0.1

*/


class spi_device {

	public:

		/* ------------------------------ */
		/* Constructor                    */
		/* ------------------------------ */
		/* spi_device: one of SPI DEVICES */
		/* spi_mode: one of SPI MODES     */
		/* spi_bitn: bits per transfer    */
		/* spi_speed: speed in HZ         */
		/* ------------------------------ */
		spi_device(
			char* spi_device,
			uint8_t spi_mode,
			uint8_t spi_bitn,
			uint32_t spi_speed
		);

		/* ------------------------------ */
		/* Half duplex write              */
		/* ------------------------------ */
		void writeBuf(uint8_t *bufdata, uint32_t bufsize);

		/* ------------------------------ */
		/* Half duplex read               */
		/* ------------------------------ */
		void readBuf(uint8_t *bufdata, uint32_t bufsize);

		/* ------------------------------ */
		/* Full duplex trasfer            */
		/* ------------------------------ */
		void transferBuf(uint8_t *rdata, uint8_t *wdata, uint32_t bufsize);

		/* ------------------------------ */
		/* Half duplex write              */
		/* ------------------------------ */
		void writeSingle(uint8_t to_transfer);

		/* ------------------------------ */
		/* Half duplex read               */
		/* ------------------------------ */
		uint8_t readSingle(void);

		/* ------------------------------ */
		/* Full duplex trasfer            */
		/* ------------------------------ */
		uint8_t transferSingle(uint8_t to_transfer);

		/* ------------------------------ */
		/* Destructor                     */
		/* ------------------------------ */
		~spi_device();
	
	private:

		/* ------------------------------ */
		/* File descriptor                */
		/* ------------------------------ */
		/* Contains the posix file        */
		/* descripor for the initialized  */
		/* device                         */
		/* ------------------------------ */
		int fd;

		/* ------------------------------ */
		/* Devide mode                    */
		/* ------------------------------ */
		/* One of the device modes as     */
		/* available in the linux kernel  */
		/* header file                    */
		/* ------------------------------ */
		uint8_t dev_mode;

		/* ------------------------------ */
		/* Devide word bits count         */
		/* ------------------------------ */
		/* Number of bits in a single     */
		/* transfer (word of a transfer   */
		/* to be precise                  */
		/* ------------------------------ */
		uint8_t dev_bitn;

		/* ------------------------------ */
		/* Device speed                   */
		/* ------------------------------ */
		/* Speed in Hz of the clock for   */
		/* the device                     */
		/* ------------------------------ */		
		uint32_t dev_speed;

};

