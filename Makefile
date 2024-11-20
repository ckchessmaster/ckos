CC		:= i686-elf-gcc
AS		:= i686-elf-as
TST_CC	:= gcc

WARNINGS := -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes

CFLAGS := -std=gnu23 $(WARNINGS) -ffreestanding -O2
LDFLAGS := -ffreestanding -O2 -nostdlib

TST_CFLAGS	:= -DUNITY_INCLUDE_CONFIG_H -DTEST -nostdlib -ffreestanding
TST_LDFLAGS := 

SRC 		:= src
BUILD 		:= build
OBJ     	:= $(BUILD)/obj
INCLUDES	:= $(wildcard $(SRC)/**/include)
BIN     	:= $(BUILD)/bin
KERNEL  	:= $(BIN)/kernel.bin
ISO			:= $(BUILD)/iso
RELEASE 	:= $(BUILD)/release/ckos.iso

TST_SRC		:= test
TST_BIN		:= $(BUILD)/test/bin
TST_OBJ 	:= $(BUILD)/test/obj
TST_RESULT	:= $(BUILD)/test/results
TST_INCLUDES:= $(INCLUDES) Unity/src $(TST_SRC)

SRCS    := $(wildcard $(SRC)/**/*.c) $(wildcard $(SRC)/**/*.S)

OBJS    := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
OBJS    := $(patsubst $(SRC)/%.S, $(OBJ)/%.o, $(OBJS))
OBJS	:= $(OBJS) $(OBJ)/external/font.o

DEPS	:= $(patsubst $(SRC)/%.c, $(OBJ)/%.d, $(SRCS))
DEPS	:= $(patsubst $(SRC)/%.S, $(OBJ)/%.d, $(DEPS))

TSTS		:= $(wildcard $(TST_SRC)/**/*.c)
TST_EXCLUDES:= $(TST_OBJ)/kernel/kernel.o $(TST_OBJ)/kernel/paging.o $(TST_OBJ)/kernel/boot.o
TST_OBJS	:= $(patsubst $(TST_SRC)/%.c, $(TST_OBJ)/%.o, $(TSTS)) $(TST_OBJ)/unity.o $(patsubst $(OBJ)/%.o, $(TST_OBJ)/%.o, $(OBJS))
TST_OBJS	:= $(filter-out $(TST_EXCLUDES), $(TST_OBJS))
TST_RESULTS := $(patsubst $(TST_SRC)/%.c, $(TST_RESULT)/%.txt, $(TSTS))

HEADERS		:= $(foreach incl, $(INCLUDES), $(wildcard $(incl)/*.h))
ALLFILES	:= $(SRCS) $(HEADERS)

PASSED 	:= `grep -s :PASS $(TST_RESULT)/**/*.txt`
FAIL 	:= `grep -s :FAIL $(TST_RESULT)/**/*.txt`
IGNORE 	:= `grep -s :IGNORE $(TST_RESULT)/**/*.txt`

-include $(DEPS)

.PHONY: all clean todo static test

all: setup $(RELEASE)

debug:
	@echo $(TST_INCLUDES)

setup:
	@mkdir -p build/obj/kernel
	@mkdir -p build/obj/libc
	@mkdir -p build/obj/external
	@mkdir -p build/bin
	@mkdir -p build/release
	@mkdir -p build/iso/boot/grub

	@mkdir -p build/test/bin
	@mkdir -p build/test/results/kernel
	@mkdir -p build/test/results/libc
	@mkdir -p build/test/obj/kernel
	@mkdir -p build/test/obj/libc
	@mkdir -p build/test/obj/external

clean:
	-@rm -R build/*

todo:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO $$file; done; true

test: setup $(TST_RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

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

# Tests-----------------------------------------------------------------------------------------------------
# Test results
$(TST_RESULT)/%.txt: $(TST_BIN)/test-runner.bin
	-./$< > $@ 2>&1

# Create test runner binary (Linker)
$(TST_BIN)/test-runner.bin: $(TST_OBJS)
	@$(TST_CC) $(TST_LDFLAGS) -o $(TST_BIN)/test-runner.bin $(TST_OBJS)

# Create obj files from tests
$(TST_OBJ)/%.o: $(TST_SRC)/%.c Makefile
	$(TST_CC) $(TST_CFLAGS) $(foreach incl, $(TST_INCLUDES), $(addprefix -I,$(incl))) -c $< -o $@

# Create obj files from src
$(TST_OBJ)/%.o: $(SRC)/%.c Makefile
	$(TST_CC) $(TST_CFLAGS) $(foreach incl, $(TST_INCLUDES), $(addprefix -I,$(incl))) -c $< -o $@

# Font obj
$(TST_OBJ)/external/font.o: font.psf Makefile
	@objcopy -O pe-x86-64 -B i386:x86-64 -I binary $< $@

# Unity obj
$(TST_OBJ)/unity.o: Unity/src/unity.c Makefile
	$(TST_CC) $(TST_CFLAGS) $(foreach incl, $(TST_INCLUDES), $(addprefix -I,$(incl))) -c $< -o $@
