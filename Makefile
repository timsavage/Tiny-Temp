#### Project configuration options ####

# Name of target controller
MCU = attiny85

# Project name
PROJECTNAME = tinytemp

# MCU Clock frequency (8Mhz)
CLK_FREQ = 8000000UL

# Source files
SRC = main.c display.c

# Additional include paths
INCLUDES =

# Libraries to link in
LIBS =

# Optimization
# use s (size opt), 1, 2, 3 or 0 (off)
OPTIMIZE = 1

# AVR Dude programmer
AVRDUDE_PROGRAMMER = usbtiny

#### End project configuration ####

#### Flags

DEFINES = -DF_CPU=$(CLK_FREQ)

# Compiler
override CFLAGS = -I. $(INCLUDES) -g -O$(OPTIMIZE) -mmcu=$(MCU) $(DEFINES) \
		-Wall -Werror -std=gnu99 \
		-fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -ffunction-sections
override CPPFLAGS = CFLAGS

# Assembler
override ASMFLAGS = -I. $(INCLUDES) -mmcu=$(MCU) $(DEFINES)

# Linker
override LDFLAGS = -Wl,-Map,$(TRG).map $(CFLAGS)

#### Executables

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AVRDUDE = avrdude
REMOVE = rm -f


#### Target Names

TRG = $(PROJECTNAME).out
DUMPTRG = $(PROJECTNAME).s

HEXROMTRG = $(PROJECTNAME).hex
HEXTRG = $(HEXROMTRG) $(PROJECTNAME).ee.hex
GDBINITFILE = gdbinit-$(PROJECTNAME)

# Filter files by type
CFILES = $(filter %.c, $(SRC))
CPPFILES =  $(filter %.cpp, $(SRC))
ASMFILES = $(filter %.S, $(SRC))

# Generate list of object files
OBJS = $(CFILES:.c=.c.o) $(ASMFILES:.S=.S.o)

# Define .lst files
LST = $(filter %.lst, $(OBJS:.o=.lst))

# Build all
all: $(TRG)

stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG)

hex: $(HEXTRG)

upload: hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -U flash:w:$(HEXROMTRG)

fuses:
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -U lfuse:w:0x62:m -U hfuse:w:0xdf:m

install: fuses upload

# Linking
$(TRG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TRG) $(OBJS)

# Generate object files
%.c.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.cpp.o: src/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

%.S.o: src/%.S
	$(CC) $(ASMFLAGS) -c $< -o $@

# Generate hex
%.hex: %.out
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.ee.hex: %.out
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

# GDB Init file
gdbinit: $(GDBINITFILE)
	@echo "file $(TRG)" > $(GDBINITFILE)
	@echo "target remote localhost:1212" >> $(GDBINITFILE)
	@echo "load" >> $(GDBINITFILE)
	@echo "break main" >> $(GDBINITFILE)
	@echo "continue" >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"

clean:
	$(REMOVE) $(TRG) $(TRG).map
	$(REMOVE) $(OBJS)
	$(REMOVE) $(GDBINITFILE)
	$(REMOVE) *.hex
