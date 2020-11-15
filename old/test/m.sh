avr-g++ -v -Wall -g -Os -mmcu=atmega328p -o main.bin main.cpp
avr-size -C main.bin
avr-objcopy -j .text -j .data -O ihex main.bin main.hex

