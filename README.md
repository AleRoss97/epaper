# epaper

Project for displaying stories with Pimoroni GFX HAT and Raspberry Pi Zero

These repository contains

- Three cpp classes:

  - spi_device: uses /dev/spi*.* to comunicate with the display (so the SPI driver must be enabled - see config.txt on raspberry pi)

  - gpio_device: direct access to memory to handle the GPIO pins

  - graphic_device: class to handle the GFX HAT display, requires spi_device and gpio_device

  - epaper: Old class to handle InkyPhat epaper display, not used anymore
  
- Two files from another repository (https://github.com/mattflow/cbmp):

  - cbmp.c: i did not write this.

  - cbmp.h: i did not write this.

## How to build

Clone the repository and run the build.sh by issuing:

  bash build.sh
  
This will generate:

- app_DisplayGrid.o: displays a set of axys on the display to check alignment. Does not require any dependencies.

- app_DisplayImage.o: displays the image "prova.bmp" to the display. Must be of the right size, if not an error is generated.

- app_GraphicTest.o: displays a moving square on the display

- app_TestButtonsLeds.o: uses directly i2c to test buttons and leds

## Issues

Opened issues:

- SPI might always read 0

## Unsupported

- Fast update on the inky phat display 

## Work in progress

This i'm working on and eventually do and update:

- Niente

## Useful links

Resource i find useful to build this:

[Display controller datasheet](https://annejan.com/media/SSD1608_1.2.pdf)

[Raspberry PI Zero W datasheet](https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)

[Ben Krasnow fast update implementation for epaper](https://benkrasnow.blogspot.com/2017/10/fast-partial-refresh-on-42-e-paper.html)

[InkyPhat/InkyWhat python library github repository](https://github.com/pimoroni/inky)

[Success thread oni fast updating for SSD1675A controllers](https://forums.pimoroni.com/t/my-inky-phat-clock-refresh-speed-question/6955/5)
