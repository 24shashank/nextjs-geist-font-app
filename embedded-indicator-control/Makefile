# Makefile for Vehicle Indicator Control System

# Toolchain
CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
AVRDUDE = avrdude

# Microcontroller
MCU = atmega328p
F_CPU = 16000000UL

# Directories
BASE_DIR = base-software
BUILD_DIR = build

# Source files
C_SOURCES = $(wildcard $(BASE_DIR)/src/*.c)
C_INCLUDES = -I$(BASE_DIR)/inc

# Compiler flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra
CFLAGS += $(C_INCLUDES)

# Linker flags
LDFLAGS = -mmcu=$(MCU)

# Output files
TARGET = indicator_control
ELF_FILE = $(BUILD_DIR)/$(TARGET).elf
HEX_FILE = $(BUILD_DIR)/$(TARGET).hex

# Default target
all: $(HEX_FILE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile C files
$(BUILD_DIR)/%.o: $(BASE_DIR)/src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(ELF_FILE): $(C_SOURCES:$(BASE_DIR)/src/%.c=$(BUILD_DIR)/%.o)
	$(CC) $(LDFLAGS) $^ -o $@

# Convert to hex
$(HEX_FILE): $(ELF_FILE)
	$(OBJCOPY) -O ihex $< $@

# Flash to microcontroller
flash: $(HEX_FILE)
	$(AVRDUDE) -c arduino -p $(MCU) -U flash:w:$(HEX_FILE)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Size information
size: $(ELF_FILE)
	$(SIZE) --format=avr --mcu=$(MCU) $<

# Include generated Simulink code
simulink:
	# This target will integrate MATLAB generated code
	# Run after Simulink code generation
	cp $(APP_DIR)/generated-code/* $(BASE_DIR)/src/

.PHONY: all clean flash size simulink
