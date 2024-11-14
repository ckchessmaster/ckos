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
ISO		:= $(BUILD)/iso
RELEASE := $(BUILD)/release/ckos.iso

SRCS    := $(wildcard $(SRC)/**/*.c)
OBJS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
OBJS	:= $(OBJS) $(OBJ)/external/font.o $(OBJ)/kernel/boot.o
HEADERS	:= $(foreach incl, $(INCLUDES), $(wildcard $(incl)/*.h))
DEPS	:= $(patsubst $(SRC)/%.c, $(OBJ)/%.d, $(SRCS))
KERNEL  := $(BIN)/kernel.bin
ALLFILES:= $(SRCS) $(HEADERS)

-include $(DEPS)

.PHONY: all clean todo static

all: setup $(RELEASE)

debug:
	@echo $(OBJ)
	@echo $(OBJS)
	@echo $(SRC)
	@echo $(SRCS)

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

# Build special content
$(OBJ)/external/font.o: font.psf
	@objcopy -O elf32-i386 -B i386 -I binary $< $@

$(OBJ)/kernel/boot.o: $(SRC)/kernel/boot.S
	@$(AS) $< -o $@

# Test
$(TST)/%_t: %.c Makefile $(RELEASE)
	@$(CC) $(CFLAGS) -MMD -MP -DTEST $< $(RELEASE) -o $@
