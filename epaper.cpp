/* -------------------------------------------------------------------------- */
/* NAME: Epaper Display CPP - Inky phat Pimoroni                              */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* SOC: SSD1608                                                               */
/* DOCUMENT REFERENCE: SSD1608_1.2.PDF                                        */
/* -------------------------------------------------------------------------- */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "epaper.h"

/* --------------------------------------------------------------------------- */
/* TIMINGS DEFINE                                                              */
/* --------------------------------------------------------------------------- */

#define MAX_BUSY_WAIT_MS 20000
#define MAX_HARD_RESET_MS 500
#define MAX_SOFT_RESET_MS 500

/* --------------------------------------------------------------------------- */
/* DISPLAY PINS                                                                */
/* --------------------------------------------------------------------------- */

#define RESET_PIN 27
#define BUSY_PIN  17
#define DC_PIN    22 /* COMAND: 0, DATA: 1 */
#define CS_PIN   0

/* --------------------------------------------------------------------------- */
/* DISPLAY COMMANDS                                                            */
/* --------------------------------------------------------------------------- */

#define CMD_SOFT_RESET       0x12
#define CMD_DRIVER_CONTROL   0x01
#define CMD_WRITE_DUMMY      0x3A
#define CMD_WRITE_GATELINE   0x3B
#define CMD_DATA_MODE        0x11
#define CMD_SET_RAMXPOS      0x44
#define CMD_SET_RAMYPOS      0x45
#define CMD_WRITE_VCOM       0x2C
#define CMD_WRITE_LUT        0x32
#define CMD_SET_RAMXCOUNT    0x4E
#define CMD_SET_RAMYCOUNT    0x4F
#define CMD_WRITE_RAM        0x24
#define CMD_READ_RAM         0x25
#define CMD_WRITE_ALTRAM     0x26
#define CMD_MASTER_ACTIVATE  0x20
#define CMD_UPDATE_SEQUENCE  0x22

#define DATA_ENA_CLK_CP      0xC0
#define DATA_INTIAL_DISPLAY  0x08
#define DATA_DISPLAY_PATTEN  0x04
#define DATA_FULL_SEQUENCE   0xFF


/* --------------------------------------------------------------------------- */
/* LUTS                                                                        */
/* --------------------------------------------------------------------------- */

const uint32_t lut_size = 30;
uint8_t lut_data[lut_size] = {
    /* 00 */ 0x02,
    /* 01 */ 0x02,
    /* 02 */ 0x01,
    /* 03 */ 0x11,
    /* 04 */ 0x12,
    /* 05 */ 0x12,
    /* 06 */ 0x22,
    /* 07 */ 0x22,
    /* 08 */ 0x66,
    /* 09 */ 0x69,
    /* 10 */ 0x69,
    /* 11 */ 0x59,
    /* 12 */ 0x58,
    /* 13 */ 0x99,
    /* 14 */ 0x99,
    /* 15 */ 0x88,
    /* 16 */ 0x00,
    /* 17 */ 0x00,
    /* 18 */ 0x00,
    /* 19 */ 0x00,
    /* 20 */ 0xF0,
    /* 21 */ 0xF0,
    /* 22 */ 0xF0,
    /* 23 */ 0xF0,
    /* 24 */ 0xF0,
    /* 25 */ 0xF0,
    /* 26 */ 0xF0,
    /* 27 */ 0xF0,
    /* 28 */ 0xF0,
    /* 29 */ 0xF0
};

/* --------------------------------------------------------------------------- */
/* HELPER FUNCTIONS                                                            */
/* --------------------------------------------------------------------------- */

static void assert(uint8_t cond, char* desc) {

	if (!cond) {
		fprintf(stderr, "\n\n[ERROR] %s\n\n\n", desc);
		exit(1);
	}

}

static void delay_ms(uint32_t ms) {
	usleep(1000*ms);
}

/* --------------------------------------------------------------------------- */
/* PUBPLIC FUNCTIONS                                                           */
/* --------------------------------------------------------------------------- */

epaper::epaper() {

	printf("[EPAP] Initialize SPI comunication.\n");
	spi = new spi_device("/dev/spidev0.0", SPI_MODE_0, 8, 100000);
	printf("[EPAP] Initialize GPIO comunication.\n");
	gpio = new gpio_device();

	printf("[EPAP] Set pins directions.\n");
	gpio->direction(BUSY_PIN, gpio->INPUT);
	gpio->direction(RESET_PIN, gpio->OUTPUT);
	gpio->direction(DC_PIN, gpio->OUTPUT);
	gpio->direction(CS_PIN, gpio->OUTPUT);

	printf("[EPAP] Set pins default states.\n");
	gpio->set(DC_PIN, LOW);
	gpio->set(RESET_PIN, HIGH);
	gpio->set(CS_PIN, HIGH);

	clear(BLACK);

	printf("[EPAP] Hard reset display.\n");
	gpio->set(RESET_PIN, LOW);
	delay_ms(MAX_HARD_RESET_MS);
	gpio->set(RESET_PIN, HIGH);
	delay_ms(MAX_HARD_RESET_MS);

	printf("[EPAP] Soft reset display.\n");
	send_command(CMD_SOFT_RESET);
	delay_ms(MAX_SOFT_RESET_MS);
	wait_while_busy();

	printf("[EPAP] Read status.\n");
	uint8_t data4 = recv_data();
	printf("[EPAP] Status is %02X\n", data4);
	wait_while_busy();

	printf("[EPAP] Configure driver.\n");
	send_command(CMD_DRIVER_CONTROL);
	send_data(rows-1);
	send_data((rows-1)>>8);
	send_data(0x00);
	wait_while_busy();

	printf("[EPAP] Set Dummy line perio.\n");
	send_both(CMD_WRITE_DUMMY, 0x1B);
	wait_while_busy();

	printf("[EPAP] Set Line Width.\n");
	send_both(CMD_WRITE_GATELINE, 0x0B);
	wait_while_busy();
	
	printf("[EPAP] Set Data entry squence .\n");
	send_both(CMD_DATA_MODE, 0x03); /* scan direction leftward and downward */
	wait_while_busy();

	printf("[EPAP] Set ram X start and end position.\n");
	send_command(CMD_SET_RAMXPOS);
	send_data(0x00);
	send_data((cols/8)-1);
	wait_while_busy();
	
	printf("[EPAP] Set ram Y start and end position.\n");
	send_command(CMD_SET_RAMYPOS);
	send_data(0x00);
	send_data(0x00);
	send_data((rows - 1));
	send_data((rows - 1) >> 8);
	wait_while_busy();
	
	printf("[EPAP] Set VCOM Voltage.\n");
	send_both(CMD_WRITE_VCOM, 0x70);
	wait_while_busy();
	
	printf("[EPAP] Write LUT DATA.\n");
	send_command(CMD_WRITE_LUT);
	uint32_t i;
	for (i = 0; i < lut_size; i++) {
		send_data(lut_data[i]);
	}

	/* Self test */
	printf("[EPAP] EPAP Self test:\n");

	printf("[EPAP]   Set RAM X address to 0.\n");
	send_command(CMD_SET_RAMXCOUNT);
	send_data(0x00);
	wait_while_busy();
	printf("[EPAP]   Set RAM Y address to 0.\n");
	send_command(CMD_SET_RAMYCOUNT);
	send_data(0x00);
	send_data(0x00);
	wait_while_busy();


	printf("[EPAP]   Set first four values of RAM.\n");
	send_command(CMD_WRITE_RAM);
	send_data(0x0A);
	send_data(0x1B);
	send_data(0x2C);
	send_data(0x3D);
	send_command(CMD_WRITE_ALTRAM);
	send_data(0x0A);
	send_data(0x1B);
	send_data(0x2C);
	send_data(0x3D);

	printf("[EPAP]   Set RAM X address to 0.\n");
	send_command(CMD_SET_RAMXCOUNT);
	send_data(0x00);
	wait_while_busy();
	printf("[EPAP]   Set RAM Y address to 0.\n");
	send_command(CMD_SET_RAMYCOUNT);
	send_data(0x00);
	send_data(0x00);
	wait_while_busy();

	printf("[EPAP]   Set first four values of RAM.\n");
	send_command(CMD_READ_RAM);
	uint8_t data0 = recv_data();
	uint8_t data1 = recv_data();
	uint8_t data2 = recv_data();
	uint8_t data3 = recv_data();

	printf("[EPAP]   Set RAM X address to 0.\n");
	send_command(CMD_SET_RAMXCOUNT);
	send_data(0x00);
	wait_while_busy();
	printf("[EPAP]   Set RAM Y address to 0.\n");
	send_command(CMD_SET_RAMYCOUNT);
	send_data(0x00);
	send_data(0x00);
	wait_while_busy();

	if (data0 != 0x0A) printf("[EPAP] SELF TEST FAILED ON BYTE 0 {%02X,%02X} !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", data0, 0x0A);
	if (data1 != 0x1B) printf("[EPAP] SELF TEST FAILED ON BYTE 1 {%02X,%02X} !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", data1, 0x1B);
	if (data2 != 0x2C) printf("[EPAP] SELF TEST FAILED ON BYTE 2 {%02X,%02X} !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", data2, 0x2C);
	if (data3 != 0x3D) printf("[EPAP] SELF TEST FAILED ON BYTE 3 {%02X,%02X} !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", data3, 0x3D);

	printf("[EPAP] End of self test.\n");

}

epaper::~epaper() {

	delete spi;
	delete gpio;

}
void epaper::show(uint8_t wait) {

	uint32_t i;

	printf("[EPAP] Converting in to packet bits.\n");
	buff_to_mem();

	printf("[EPAP] Set RAM X address to 0.\n");
	send_command(CMD_SET_RAMXCOUNT);
	send_data(0x00);
	wait_while_busy();

	printf("[EPAP] Set RAM Y address to 0.\n");
	send_command(CMD_SET_RAMYCOUNT);
	send_data(0x00);
	send_data(0x00);
	wait_while_busy();


	printf("[EPAP] Set Black/white buffer.\n");
	send_command(CMD_WRITE_RAM);
	for (i = 0; i < mem_len; i++) {
		send_data(mem_data[0][i]);
	}

	printf("[EPAP] Set Yellow/white buffer.\n");
	send_command(CMD_WRITE_ALTRAM);
	for (i = 0; i < mem_len; i++) {
		send_data(mem_data[1][i]);
	}

	printf("[EPAP] Master activate.\n");
	send_command(CMD_MASTER_ACTIVATE);
	if (wait == 1) {
		wait_while_busy();
	}
}

void epaper::clear(uint8_t color) {

	printf("[EPAP] Clear buffers %s.\n", color == BLACK ? "BLACK": color == WHITE ? "WHITE": "YELLOW");
	uint32_t x,y;
	for (x = 0; x < width; x++) {
		for (y = 0; y < heigth; y++) {
			buff[x][y] = color;
		}
	}

}

/* --------------------------------------------------------------------------- */
/* PRIVATE FUNCTIONS                                                           */
/* --------------------------------------------------------------------------- */

void epaper::buff_to_mem(void) {

	uint32_t currbit = 0;
	uint32_t x,y;
	for (y = 0; y < heigth; y++) {
		for (x = 0; x < width; x++) {
			
			uint32_t currbyte = currbit/8;
			uint32_t curroffs = currbit%8;
			curroffs = 7-curroffs;

			assert(currbyte < mem_len, "[EPAP] buff_to_mem not working");

			switch (buff[x][y]) {
				case WHITE:
					mem_data[0][currbyte] |= (1 << curroffs);
					mem_data[1][currbyte] &= ~(1 << curroffs);
				break;
				case BLACK:
					mem_data[0][currbyte] &= ~(1 << curroffs);
					mem_data[1][currbyte] &= ~(1 << curroffs);
				break;
				case YELLOW:
					mem_data[0][currbyte] &= ~(1 << curroffs);
					mem_data[1][currbyte] |= (1 << curroffs);
				break;

			}

			/* Increment idx */
			currbit++;

		}
	}

}

void epaper::send_both(uint8_t cmd, uint8_t data) {

	send_command(cmd);
	send_data(data);

}
void epaper::send_command(uint8_t cmd) {

	gpio->set(DC_PIN,  LOW);
	spi->writeSingle(cmd);

}
void epaper::send_data(uint8_t data) {

	gpio->set(DC_PIN,  HIGH);
	spi->writeSingle(data);

}
uint8_t epaper::recv_data() {

	gpio->set(DC_PIN,  HIGH);
	return spi->readSingle();

}

void epaper::wait_while_busy() {

	uint32_t times = 0;

	while (gpio->get(BUSY_PIN) == 1) {
		delay_ms(10);	
		times++;
		//assert((times*10) < MAX_BUSY_WAIT_MS, "Busy line high for too much.");
		if ((times*10) > 10000) break;
	}

}
