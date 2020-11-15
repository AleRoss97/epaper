# ---------------------------------------------------------------------------- #
# Build file for Display projects                                              #
# ---------------------------------------------------------------------------- #
# AUTHOR: Alessandro Rossetti                                                  #
# VERSION: 0000                                                                #
# ---------------------------------------------------------------------------- #
# DESCRIPTION                                                                  #
# - Compiles the libraries to object files                                     #
# - Compiles the main programs to object files                                 #
# - Links the libraries object files and the DisplayGrid object files          #
# - Links the libraries object files and the DisplayImage object files         #
# ---------------------------------------------------------------------------- #

clear;

# ---------------------------------------------------------------------------- #
# Compiling libraries                                                          #
# ---------------------------------------------------------------------------- #

echo

echo Compiling epaper library
g++ -Wno-write-strings -g -c epaper.cpp

echo Compiling gpio device
g++ -Wno-write-strings -g -c gpio_device.cpp

echo Compiling spi device
g++ -Wno-write-strings -g -c spi_device.cpp

echo Compiling bmp library
g++ -Wno-write-strings -g -c cbmp.c -o cbmp.o

echo Compiling graphic library
g++ -Wno-write-strings -g -c graphic_device.cpp -o graphic_device.o

# ---------------------------------------------------------------------------- #
# Compiling main programs                                                      #
# ---------------------------------------------------------------------------- #

echo

echo Compiling main_DisplayImage
g++ -fpermissive -Wno-write-strings -g -c main_DisplayImage.cpp -o main_DisplayImage.o

echo Compiling main_DisplayGrid.cpp
g++ -fpermissive -Wno-write-strings -g -c main_DisplayGrid.cpp -o main_DisplayGrid.o

echo Compiling main_GraphicTest.cpp
g++ -fpermissive -Wno-write-strings -g -c main_GraphicTest.cpp -o main_GraphicTest.o

# ---------------------------------------------------------------------------- #
# Links main programs                                                          #
# ---------------------------------------------------------------------------- #

echo

echo Linking DisplayImage
g++ -g epaper.o gpio_device.o spi_device.o cbmp.o main_DisplayImage.o -o app_DisplayImage.o

echo Linking DisplayGrid
g++ -g epaper.o gpio_device.o spi_device.o cbmp.o main_DisplayGrid.o -o app_DisplayGrid.o

echo Linking GraphicTest
g++ -g graphic_device.o gpio_device.o spi_device.o main_GraphicTest.o -o app_GraphicTest.o

# ---------------------------------------------------------------------------- #
# End of program                                                               #
# ---------------------------------------------------------------------------- #
