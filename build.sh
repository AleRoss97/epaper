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

function compile {

	if test -f "$1.o"
	then
		echo Not compiling $1
	else
		echo Compiling $1
		
		if test -f "$1.cpp"
		then
			g++ -Wno-write-strings -g -c $1.cpp -o $1.o
		else
			g++ -Wno-write-strings -g -c $1.c -o $1.o
		fi

	fi

}

if [ -z "$1" ]
then
    echo "Compiling only missing files..."
else
	if [ "$1" = "all" ]
	then
    	echo "Removed all output files"
		rm *.o
	else
		echo "Removing file $1"
		rm $1.o
	fi
fi

# ---------------------------------------------------------------------------- #
# Compiling libraries                                                          #
# ---------------------------------------------------------------------------- #

echo
compile epaper
compile gpio_device
compile spi_device
compile cbmp
compile graphic_device
compile i2c_device

# ---------------------------------------------------------------------------- #
# Compiling main programs                                                      #
# ---------------------------------------------------------------------------- #

echo

compile main_DisplayImage
compile main_DisplayGrid
compile main_GraphicTest
compile main_TestButtonsLeds

# ---------------------------------------------------------------------------- #
# Links main programs                                                          #
# ---------------------------------------------------------------------------- #

echo

echo Linking DisplayImage
g++ -g epaper.o gpio_device.o spi_device.o cbmp.o main_DisplayImage.o -li2c -o app_DisplayImage.o

echo Linking DisplayGrid
g++ -g epaper.o gpio_device.o spi_device.o cbmp.o main_DisplayGrid.o -li2c -o app_DisplayGrid.o

echo Linking GraphicTest
g++ -g graphic_device.o gpio_device.o spi_device.o main_GraphicTest.o -li2c -o app_GraphicTest.o

echo Linking TestButtonsLeds
g++ -g graphic_device.o gpio_device.o spi_device.o i2c_device.o main_TestButtonsLeds.o -li2c -o app_TestButtonsLeds.o

# ---------------------------------------------------------------------------- #
# End of program                                                               #
# ---------------------------------------------------------------------------- #
