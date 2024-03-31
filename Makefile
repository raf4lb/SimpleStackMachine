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
	$(eval PYTHON_SCRIPT := compiler.py)
	@echo "Compiling rfl file..."
	$(eval OUTPUT := $(shell python3 $(PYTHON_SCRIPT) $(PROGRAM_FILE) $(PORTS_SIZE)))
	@echo "OK"
	$(eval PROGRAM := $(word 1, $(OUTPUT)))
	$(eval PROGRAM_SIZE := $(word 2, $(OUTPUT)))
	@echo "Program: $(PROGRAM)"
	@echo "Size: $(PROGRAM_SIZE)"
	mkdir -p $(BUILD_DIR)
	@echo "Sending code to RFLVM"
	avr-gcc -Os -mmcu=atmega328p \
		$(SOURCE_DIR)/memory.c \
		$(SOURCE_DIR)/io.c \
		$(SOURCE_DIR)/delay.c \
		$(SOURCE_DIR)/stack.c \
		$(SOURCE_DIR)/cpu.c \
		$(SOURCE_DIR)/instructions.c \
		$(SOURCE_DIR)/serial.c \
		$(SOURCE_DIR)/main.c \
		-DPROGRAM=\""$(PROGRAM)"\" -DPROGRAM_SIZE=$(PROGRAM_SIZE) \
		-o build/$@.bin
	avr-objcopy -O ihex -R .eeprom build/$@.bin build/$@.hex

clean:
	rm -r $(BUILD_DIR)
	