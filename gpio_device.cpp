/* -------------------------------------------------------------------------- */
/* NAME: GPIO Device CPP - Raspberry Pi Zero W                                */
/* -------------------------------------------------------------------------- */
/* AUTHOR: Alessandro Rossetti                                                */
/* VERSION: 0000                                                              */
/* SOC: BCM2835                                                               */
/* DOCUMENT REFERENCE: BCM2835-ARM-Peripherals.pdf                            */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>    //for error handling
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include "gpio_device.h"

static void assert(uint8_t cond, char* desc) {

	if (!cond) {
		fprintf(stderr, "\n\n[ERROR] %s\n\n\n", desc);
		exit(1);
	}

}

/* -------------------------------------------------------------------------- */
/* ALTERNATE FUNCTIONS                                                        */
/* -------------------------------------------------------------------------- */
#define REG_GPFSEL0    0x7E200000  /* R/W  GPIO Function Select 0             */
#define REG_GPFSEL1    0x7E200004  /* R/W  GPIO Function Select 1             */
#define REG_GPFSEL2    0x7E200008  /* R/W  GPIO Function Select 2             */
#define REG_GPFSEL3    0x7E20000C  /* R/W  GPIO Function Select 3             */
#define REG_GPFSEL4    0x7E200010  /* R/W  GPIO Function Select 4             */
#define REG_GPFSEL5    0x7E200014  /* R/W  GPIO Function Select 5             */
/* -------------------------------------------------------------------------- */
/* OUTPUT REGISTERS                                                           */
/* -------------------------------------------------------------------------- */
#define REG_GPSET0     0x7E20001C  /* W    GPIO Pin Output Set 0              */
#define REG_GPSET1     0x7E200020  /* W    GPIO Pin Output Set 1              */
#define REG_GPCLR0     0x7E200028  /* W    GPIO Pin Output Clear 0            */
#define REG_GPCLR1     0x7E20002C  /* W    GPIO Pin Output Clear 1            */
/* -------------------------------------------------------------------------- */
/* INPUT REGISTERS                                                            */
/* -------------------------------------------------------------------------- */
#define REG_GPLEV0     0x7E200034  /* R    GPIO Pin Level 0                   */
#define REG_GPLEV1     0x7E200038  /* R    GPIO Pin Level 1                   */
/* -------------------------------------------------------------------------- */
/* PULL CONTROL                                                               */
/* -------------------------------------------------------------------------- */
#define REG_GPPUD      0x7E200094  /* R/W  GPIO Pin Pull-up/down Enable       */
#define REG_GPPUDCLK0  0x7E200098  /* R/W  GPIO Pin Pull-up/down Enable Clock */
#define REG_GPPUDCLK1  0x7E20009C  /* R/W  GPIO Pin Pull-up/down Enable Clock */


gpio_device::gpio_device() {

	printf("[GPIO] Mapping memory.\n");

	mem_fd = open("/dev/gpiomem", O_RDWR);
	assert(mem_fd >= 0, "Unable to open memory file.");
	uint32_t* start = (uint32_t *)mmap(0, 4, PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, 0);
	if (start == MAP_FAILED) fprintf(stderr, "%d - %s\n", errno, strerror(errno));
	assert(start != MAP_FAILED, "Unable to access memory at address gpio_fsel[0]");

	uint32_t start_addr = (uint32_t)start;

	gpio_fsel[0]   = (uint32_t*)(start_addr+(REG_GPFSEL0-REG_GPFSEL0));
	gpio_fsel[1]   = (uint32_t*)(start_addr+(REG_GPFSEL1-REG_GPFSEL0));
	gpio_fsel[2]   = (uint32_t*)(start_addr+(REG_GPFSEL2-REG_GPFSEL0));
	gpio_fsel[3]   = (uint32_t*)(start_addr+(REG_GPFSEL3-REG_GPFSEL0));
	gpio_fsel[4]   = (uint32_t*)(start_addr+(REG_GPFSEL4-REG_GPFSEL0));
	gpio_fsel[5]   = (uint32_t*)(start_addr+(REG_GPFSEL5-REG_GPFSEL0));
	gpio_set[0]    = (uint32_t*)(start_addr+(REG_GPSET0-REG_GPFSEL0));
	gpio_set[1]    = (uint32_t*)(start_addr+(REG_GPSET1-REG_GPFSEL0));
	gpio_clr[0]    = (uint32_t*)(start_addr+(REG_GPCLR0-REG_GPFSEL0));
	gpio_clr[1]    = (uint32_t*)(start_addr+(REG_GPCLR1-REG_GPFSEL0));
	gpio_lev[0]    = (uint32_t*)(start_addr+(REG_GPLEV0-REG_GPFSEL0));
	gpio_lev[1]    = (uint32_t*)(start_addr+(REG_GPLEV1-REG_GPFSEL0));
	gpio_pud       = (uint32_t*)(start_addr+(REG_GPPUD-REG_GPFSEL0));
	gpio_pudclk[0] = (uint32_t*)(start_addr+(REG_GPPUDCLK0-REG_GPFSEL0));
	gpio_pudclk[1] = (uint32_t*)(start_addr+(REG_GPPUDCLK1-REG_GPFSEL0));

	printf("[GPIO] | ------------- | ---------- | ---------- | ---------- | ---------- |\n");
	printf("[GPIO] | Name          | Original   | Offset     | Mapped     | Value      |\n");
	printf("[GPIO] | ------------- | ---------- | ---------- | ---------- | ---------- |\n");
	printf("[GPIO] | REG_GPFSEL0   | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPFSEL0,   (REG_GPFSEL0-REG_GPFSEL0),   gpio_fsel[0],   (*gpio_fsel[0])   );
	printf("[GPIO] | REG_GPFSEL1   | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPFSEL1,   (REG_GPFSEL1-REG_GPFSEL0),   gpio_fsel[1],   (*gpio_fsel[1])   );
	printf("[GPIO] | REG_GPFSEL2   | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPFSEL2,   (REG_GPFSEL2-REG_GPFSEL0),   gpio_fsel[2],   (*gpio_fsel[2])   );
	printf("[GPIO] | REG_GPFSEL3   | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPFSEL3,   (REG_GPFSEL3-REG_GPFSEL0),   gpio_fsel[3],   (*gpio_fsel[3])   );
	printf("[GPIO] | REG_GPFSEL4   | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPFSEL4,   (REG_GPFSEL4-REG_GPFSEL0),   gpio_fsel[4],   (*gpio_fsel[4])   );
	printf("[GPIO] | REG_GPFSEL5   | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPFSEL5,   (REG_GPFSEL5-REG_GPFSEL0),   gpio_fsel[5],   (*gpio_fsel[5])   );
	printf("[GPIO] | REG_GPSET0    | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPSET0,    (REG_GPSET0-REG_GPFSEL0),    gpio_set[0],    (*gpio_set[0])    );
	printf("[GPIO] | REG_GPSET1    | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPSET1,    (REG_GPSET1-REG_GPFSEL0),    gpio_set[1],    (*gpio_set[1])    );
	printf("[GPIO] | REG_GPCLR0    | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPCLR0,    (REG_GPCLR0-REG_GPFSEL0),    gpio_clr[0],    (*gpio_clr[0])    );
	printf("[GPIO] | REG_GPCLR1    | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPCLR1,    (REG_GPCLR1-REG_GPFSEL0),    gpio_clr[1],    (*gpio_clr[1])    );
	printf("[GPIO] | REG_GPLEV0    | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPLEV0,    (REG_GPLEV0-REG_GPFSEL0),    gpio_lev[0],    (*gpio_lev[0])    );
	printf("[GPIO] | REG_GPLEV1    | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPLEV1,    (REG_GPLEV1-REG_GPFSEL0),    gpio_lev[1],    (*gpio_lev[1])    );
	printf("[GPIO] | REG_GPPUD     | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPPUD,     (REG_GPPUD-REG_GPFSEL0),     gpio_pud,       (*gpio_pud)       );
	printf("[GPIO] | REG_GPPUDCLK0 | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPPUDCLK0, (REG_GPPUDCLK0-REG_GPFSEL0), gpio_pudclk[0], (*gpio_pudclk[0]) );
	printf("[GPIO] | REG_GPPUDCLK1 | 0x%08X | 0x%08X | 0x%08X | 0x%08X |\n", REG_GPPUDCLK1, (REG_GPPUDCLK1-REG_GPFSEL0), gpio_pudclk[1], (*gpio_pudclk[1]) );
	printf("[GPIO] | ------------- | ---------- | ---------- | ---------- | ---------- |\n");
}

gpio_device::~gpio_device() {

	int status_0;
	int status_1;
	int status_2;
	int status_3;
	int status_4;
	int status_5;
	int status_6;
	int status_7;
	int status_8;
	int status_9;
	int status_10;
	int status_11;
	int status_26;
	int status_27;
	int status_28;

	printf("[GPIO] De allocating memory.\n");

	status_0 = munmap(gpio_fsel[0], 4);
	status_1 = munmap(gpio_fsel[1], 4);
	status_2 = munmap(gpio_fsel[2], 4);
	status_3 = munmap(gpio_fsel[3], 4);
	status_4 = munmap(gpio_fsel[4], 4);
	status_5 = munmap(gpio_fsel[5], 4);
	status_6 = munmap(gpio_set[0], 4);
	status_7 = munmap(gpio_set[1], 4);
	status_8 = munmap(gpio_clr[0], 4);
	status_9 = munmap(gpio_clr[1], 4);
	status_10 = munmap(gpio_lev[0], 4);
	status_11 = munmap(gpio_lev[1], 4);
	status_26 = munmap(gpio_pud, 4);
	status_27 = munmap(gpio_pudclk[0], 4);
	status_28 = munmap(gpio_pudclk[1], 4);

	printf("[GPIO] Checking de allocation.\n");

	assert(status_0 >= 0, "Unable to disable access at memory address gpio_fsel[0]");
	assert(status_1 >= 0, "Unable to disable access at memory address gpio_fsel[1]");
	assert(status_2 >= 0, "Unable to disable access at memory address gpio_fsel[2]");
	assert(status_3 >= 0, "Unable to disable access at memory address gpio_fsel[3]");
	assert(status_4 >= 0, "Unable to disable access at memory address gpio_fsel[4]");
	assert(status_5 >= 0, "Unable to disable access at memory address gpio_fsel[5]");
	assert(status_6 >= 0, "Unable to disable access at memory address gpio_set[0]");
	assert(status_7 >= 0, "Unable to disable access at memory address gpio_set[1]");
	assert(status_8 >= 0, "Unable to disable access at memory address gpio_clr[0]");
	assert(status_9 >= 0, "Unable to disable access at memory address gpio_clr[1]");
	assert(status_10 >= 0, "Unable to disable access at memory address gpio_lev[0]");
	assert(status_11 >= 0, "Unable to disable access at memory address gpio_lev[1]");
	assert(status_26 >= 0, "Unable to disable access at memory address gpio_pudclk[0]");
	assert(status_27 >= 0, "Unable to disable access at memory address gpio_pudclk[0]");
	assert(status_28 >= 0, "Unable to disable access at memory address gpio_pudclk[1]");

}

void gpio_device::direction(uint32_t pin, direction_t dir) {

	assert(pin <= 53, "Pin value too high.");

	printf("[GPIO] Calculate position in memory.\n");

	uint32_t register_num = pin / 10;
	uint32_t field_num    = pin % 10;

	assert(register_num <= 5, "Register value too high.");
	assert(field_num <= 9, "Field value too high.");

	printf("[GPIO] Calculate value.\n");

	uint32_t value = (uint32_t)dir;

	printf("[GPIO] Create access medium.\n");

	uint32_t* register_addr = gpio_fsel[register_num];
	uint32_t register_offs = field_num *3;

	printf("[GPIO] Write to memory the new GPIO direction.\n");

	uint32_t curr_value = (*register_addr);
	curr_value = curr_value & (~(0x00000000 | (0x07 << register_offs)));
	curr_value = curr_value | (value << register_offs);
	(*register_addr) = curr_value;


}

void gpio_device::set(uint32_t pin, uint8_t state) {

	assert(pin <= 53, "Pin value too high.");
	assert(state == LOW || state == HIGH, "State is not correct.");

	/* Calculate position in memory */

	uint32_t register_num = pin / 32;
	uint32_t field_num    = pin % 32;

	assert(register_num <= 1, "Register value too high.");
	assert(field_num <= 32, "Field value too high.");

	/* Calculate value */

	uint32_t** base_addr;
	if (state == LOW) {
		base_addr = gpio_clr;
	}
	else {
		base_addr = gpio_set;
	}

	/* Create access medium */

	uint32_t* register_addr = base_addr[register_num];
	uint32_t register_offs = field_num * 1;

	/* Write to memory */

	(*register_addr) = (1 << register_offs);

}

uint8_t gpio_device::get(uint32_t pin) {

	assert(pin <= 53, "Pin value too high.");

	/* Calculate position in memory. */

	uint32_t register_num = pin / 32;
	uint32_t field_num    = pin % 32;

	assert(register_num <= 1, "Register value too high.");
	assert(field_num <= 32, "Field value too high.");

	/* Create access medium. */

	uint32_t* register_addr = gpio_lev[register_num];
	uint32_t register_offs = field_num * 1;

	/* Write to memory. */
	
	uint32_t value = (*register_addr);
	value = (value & (1 << register_offs));

	if (value == 0) {
		return LOW;
	}
	else {
		return HIGH;
	}

	assert(false, "Control flow issue.");

}

void gpio_device::pull(uint32_t pin, pull_t type) {

	assert(pin <= 53, "Pin value too high.");

	printf("[GPIO] Calculate position in memory.\n");

	uint32_t register_num = pin / 32;
	uint32_t field_num    = pin % 32;

	assert(register_num <= 1, "Register value too high.");
	assert(field_num <= 32, "Field value too high.");

	/* Create access medium */

	uint32_t* register_control_addr = gpio_pud;
	uint32_t* register_clock_addr   = gpio_pudclk[register_num];
	uint32_t register_offs = field_num * 1;

	printf("[GPIO] Write to memory - sequence is described at page 101.\n");

	if (type == DOWN) {
		(*register_control_addr) = 0x00000001;
	}
	else if (type == UP) {
		(*register_control_addr) = 0x00000002;
	}
	else if (type == REMOVE) {
		(*register_control_addr) = 0x00000000;
	}
	else {
		assert(false, "Type is not allowed.");
	}

	printf("[GPIO] Wait for 150 cycles.\n");

	(*register_clock_addr) = (1 << register_offs);

	printf("[GPIO] Wait for 150 cycles.\n");

	(*register_control_addr) = 0x00000000;
	(*register_clock_addr) = 0x00000000;

	printf("[GPIO] Pull sequence finished.\n");

}
