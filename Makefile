# spice file netlist fixer
# Huang Rui <vowstar@gmail.com>

OUTPUT_DIR = build
SOURCES = $(wildcard *.c)
CC=gcc
CFLAGS=-I. -lm

OBJS = $(patsubst %.c, $(OUTPUT_DIR)/%.o, $(notdir $(SOURCES)))

PRINT_BUILD = @echo "Building $< -> $@ ..."
PRINT_CLEAN = @echo "Clearing build files ..."

all: pre-build $(OBJS) $(OUTPUT_DIR)/$(notdir $(shell pwd)) post-build

pre-build:
	$(PRINT_BUILD)
	@mkdir -p $(OUTPUT_DIR)

post-build:
	$(PRINT_BUILD)

$(OUTPUT_DIR)/%.o:%.c
	$(PRINT_BUILD)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUT_DIR)/$(notdir $(shell pwd)): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean all
clean:
	$(PRINT_CLEAN)
	@rm -rf $(OUTPUT_DIR)
