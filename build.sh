#!/bin/bash

rm -R build/*

i686-elf-as src/kernel/boot.S -o build/boot.o
i686-elf-gcc -c src/kernel/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T src/kernel/linker.ld -o build/ckos-kernel.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc

mkdir -p build/iso/boot/grub
mkdir -p build/iso/boot/grub/fonts
cp build/ckos-kernel.bin build/iso/boot/ckos-kernel.bin
cp src/kernel/grub.cfg build/iso/boot/grub/grub.cfg
cp font.psf build/iso/boot/grub/fonts/font.psf
grub-mkrescue -o ckos.iso build/iso
