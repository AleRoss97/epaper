# epaper

Lbrary for using the Pimoroni InkyPhat display with SSD 1608 controller in C++ language on the Raspberry Pi Zero W.

These repository contains
- Three cpp classes:

  - spi_device: uses /dev/spi*.* to comunicate with the display (so the SPI driver must be enabled - see config.txt on raspberry pi)

  - gpio_device: direct access to memory to handle the GPIO pins

  - epaper: Provides a buffer and the functions to transfer that buffer to the display
  
- Two files from another repository (https://github.com/mattflow/cbmp):

  - cbmp.c: i did not write this.

  - cbmp.h: i did not write this.

## How to build

Clone the repository and run the build.sh by issuing:

  bash build.sh
  
This will generate:

- app_DisplayGrid.o: displays a set of axys on the display to check alignment. Does not require any dependencies.

- app_DisplayImage.o: displays the image "prova.bmp" to the display. Must be of the right size, if not an error is generated.

## Issues

Opened issues:

- Reading from the display always reads 0 - This is not a big issue since no values need to be red from the display, however **this makes the self test fail** but everything else works ok.

## Work in progress

This i'm working on and eventually do and update:

- Fast update - I ported python code to c++ to implement this.

## Useful links

Resource i find useful to build this:

[Display controller datasheet](https://annejan.com/media/SSD1608_1.2.pdf)

[Raspberry PI Zero W datasheet](https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)

[Ben Krasnow fast update implementation for epaper](https://benkrasnow.blogspot.com/2017/10/fast-partial-refresh-on-42-e-paper.html)

[InkyPhat/InkyWhat python library github repository](https://github.com/pimoroni/inky)
