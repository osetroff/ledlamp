#avrdude -v -v -p atmega328p -c avrispmkii -U flash:w:main.hex:i -F -P /dev/ttyUSB0
avrdude -v -D -patmega328p -carduino "-P/dev/ttyUSB0" -b38400 "-Uflash:w:main.hex:i"