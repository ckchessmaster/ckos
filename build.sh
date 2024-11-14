#!/bin/bash

rm -R build/*
rm ckos.iso
rm ckos.log

objcopy -O elf32-i386 -B i386 -I binary font.psf build/font.o
i686-elf-as src/kernel/boot.S -o build/boot.o
i686-elf-gcc -c src/kernel/io.c -o build/io.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I src/kernel/include
i686-elf-gcc -c src/kernel/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I src/kernel/include
i686-elf-gcc \
    -T src/kernel/linker.ld \
    -o build/ckos-kernel.bin \
    -ffreestanding \
    -O2 \
    -nostdlib build/boot.o build/kernel.o build/font.o build/io.o \
    -lgcc

mkdir -p build/iso/boot/grub
cp build/ckos-kernel.bin build/iso/boot/ckos-kernel.bin
cp src/kernel/grub.cfg build/iso/boot/grub/grub.cfg
grub-mkrescue -o ckos.iso build/iso
