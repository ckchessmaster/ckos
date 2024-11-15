CC := i686-elf-gcc
AS := i686-elf-as

WARNINGS := -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes -Werror

CFLAGS := -std=gnu23 $(WARNINGS) -ffreestanding -O2
LDFLAGS := -ffreestanding -O2 -nostdlib

SRC 	:= src
BUILD 	:= build
OBJ     := $(BUILD)/obj
TST		:= $(BUILD)/test
INCLUDES:= $(wildcard $(SRC)/**/include)
BIN     := $(BUILD)/bin
KERNEL  := $(BIN)/kernel.bin
ISO		:= $(BUILD)/iso
RELEASE := $(BUILD)/release/ckos.iso

SRCS    := $(wildcard $(SRC)/**/*.c) $(wildcard $(SRC)/**/*.S)
OBJS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
OBJS    := $(patsubst $(SRC)/%.S, $(OBJ)/%.o, $(OBJS))
OBJS	:= $(OBJS) $(OBJ)/external/font.o

DEPS	:= $(patsubst $(SRC)/%.c, $(OBJ)/%.d, $(SRCS))
DEPS	:= $(patsubst $(SRC)/%.S, $(OBJ)/%.d, $(DEPS))

HEADERS	:= $(foreach incl, $(INCLUDES), $(wildcard $(incl)/*.h))
ALLFILES:= $(SRCS) $(HEADERS)

-include $(DEPS)

.PHONY: all clean todo static

all: setup $(RELEASE)

debug:
	@echo $(OBJS)
	@echo $(SRCS)
	@echo $(DEPS)

setup:
	@mkdir -p build/obj/kernel
	@mkdir -p build/obj/libc
	@mkdir -p build/obj/external
	@mkdir -p build/bin
	@mkdir -p build/release
	@mkdir -p build/iso/boot/grub

clean:
	-@rm -R build/*

todo:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO $$file; done; true

# Create ISO
$(RELEASE): $(KERNEL) $(SRC)/kernel/grub.cfg
	@cp $(KERNEL) $(ISO)/boot/kernel.bin
	@cp $(SRC)/kernel/grub.cfg $(ISO)/boot/grub/grub.cfg
	@grub-mkrescue -o $(RELEASE) $(ISO)

# Linker
$(KERNEL): $(OBJS)
	@$(CC) -T $(SRC)/kernel/linker.ld -o $(KERNEL) $(LDFLAGS) $(OBJS) -lgcc

# Build
$(OBJ)/%.o: $(SRC)/%.c Makefile
	@$(CC) $(CFLAGS) $(foreach incl, $(INCLUDES), $(addprefix -I ,$(incl))) -MMD -MP -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.S
	@$(AS) $< -o $@

# Build special content
$(OBJ)/external/font.o: font.psf
	@objcopy -O elf32-i386 -B i386 -I binary $< $@

# Test
$(TST)/%_t: %.c Makefile $(RELEASE)
	@$(CC) $(CFLAGS) -MMD -MP -DTEST $< $(RELEASE) -o $@
