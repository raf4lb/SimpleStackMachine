all: main

CC = clang
override CFLAGS += -g -Wno-everything -pthread -lm

BUILD_DIR = build
SOURCE_DIR = src

generic:
	mkdir -p build
	$(CC) $(SOURCE_DIR)/memory.c \
		$(SOURCE_DIR)/stack.c \
		$(SOURCE_DIR)/cpu.c \
		$(SOURCE_DIR)/instructions.c \
		$(SOURCE_DIR)/main.c \
		-o $(BUILD_DIR)/$@

arduino:
	mkdir -p build
	avr-gcc -Os -mmcu=atmega328p \
		$(SOURCE_DIR)/memory.c \
		$(SOURCE_DIR)/stack.c \
		$(SOURCE_DIR)/cpu.c \
		$(SOURCE_DIR)/instructions.c \
		$(SOURCE_DIR)/serial.c \
		$(SOURCE_DIR)/main.c \
		-o build/$@.bin
	avr-objcopy -O ihex -R .eeprom build/$@.bin build/$@.hex

clean:
	rm -r $(BUILD_DIR)
	