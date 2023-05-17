#
# Makefile
#
FILENAME   = led
PORT       = /dev/cu.usbserial-A600BOBB
DEVICE     = atmega328p
PROGRAMMER = arduino
# BAUD can be 115200, 74888, 57600
BAUD       = 57600
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)

default: compile upload clean

compile:
	$(COMPILE) -o $(FILENAME) $(FILENAME).s

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME)

clean:
	rm $(FILENAME)
