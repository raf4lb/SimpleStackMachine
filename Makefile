all: main

BUILD_DIR = build
SOURCE_DIR = src
TEST_DIR = tests
TEST_BUILD_DIR = $(TEST_DIR)/build

PARAMS = $(SOURCE_DIR)/memory.c \
		$(SOURCE_DIR)/io.c \
		$(SOURCE_DIR)/delay.c \
		$(SOURCE_DIR)/stack.c \
		$(SOURCE_DIR)/cpu.c \
		$(SOURCE_DIR)/instructions.c \
		$(SOURCE_DIR)/serial.c \
		$(SOURCE_DIR)/builtin.c \
		$(SOURCE_DIR)/sys.c \
		$(SOURCE_DIR)/datatypes.c \
		$(SOURCE_DIR)/main.c \
		-DPROGRAM_SIZE=$(PROGRAM_SIZE) \
		-DPROGRAM="$(PROGRAM)" \
		-DDATA_ADDRESS=$(DATA_ADDRESS) \

compile:
	mkdir -p $(BUILD_DIR)
	$(eval PYTHON_SCRIPT := compiler.py)
	@echo "Compiling rfl file..."
	$(eval OUTPUT := $(shell python3 $(PYTHON_SCRIPT) $(PROGRAM_FILE)))
	@echo "OK"
	$(eval PROGRAM := $(word 1, $(OUTPUT)))
	$(eval PROGRAM_SIZE := $(word 2, $(OUTPUT)))
	$(eval DATA_ADDRESS := $(word 3, $(OUTPUT)))
	@echo "Program: $(PROGRAM)"
	@echo "Size: $(PROGRAM_SIZE)"
	@echo "Data: $(DATA_ADDRESS)"
		
generic: compile
	@echo "Compiling code to MACOSX version"
	clang $(PARAMS) -DMACOSX -o build/$@
	@echo "MACOSX version created"

arduino: compile
	@echo "Compiling code to ARDUINO version"
	avr-gcc $(PARAMS) -Os -mmcu=atmega328p -DARDUINO -DSERIAL_ENABLED -o build/$@.elf
	avr-objcopy -O ihex -R .eeprom build/$@.elf build/$@.hex
	avr-size -C --mcu=atmega328p build/$@.elf 
	@echo "ARDUINO version created"

clean:
	rm -r $(BUILD_DIR)

test:
	mkdir -p $(TEST_BUILD_DIR)
	$(CC) \
		$(SOURCE_DIR)/memory.c \
		$(SOURCE_DIR)/io.c \
		$(SOURCE_DIR)/delay.c \
		$(SOURCE_DIR)/stack.c \
		$(SOURCE_DIR)/cpu.c \
		$(SOURCE_DIR)/instructions.c \
		$(SOURCE_DIR)/serial.c \
		$(SOURCE_DIR)/builtin.c \
		$(TEST_DIR)/test_*.c \
		-DMACOSX \
		-o $(TEST_BUILD_DIR)/$@

clean-test:
	rm -r $(TEST_BUILD_DIR)	