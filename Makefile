MCU = atmega328p
F_CPU = 16000000UL
PORT = $(shell (ls /dev/cu.usbserial-* 2>/dev/null || ls /dev/cu.usbmodem* 2>/dev/null) | head -n 1)
BAUD = 115200

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -Wall -Wextra -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)

all: main.hex

main.elf: main.c
	$(CC) $(CFLAGS) -o main.elf main.c

main.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom main.elf main.hex

flash: main.hex
	@if [ -z "$(PORT)" ]; then \
		echo "Erro: Arduino não encontrado! Verifique o cabo USB."; \
		exit 1; \
	fi
	$(AVRDUDE) -V \
		-c arduino \
		-p m328p \
		-P $(PORT) \
		-b $(BAUD) \
		-U flash:w:main.hex:i

size: main.elf
	avr-size -C --mcu=$(MCU) main.elf

clean:
	rm -f main.elf main.hex