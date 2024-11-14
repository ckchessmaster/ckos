#!/bin/bash

make all
qemu-system-x86_64 \
    -m 128 \
    -no-reboot \
    -cdrom build/release/ckos.iso \
    -nographic \
    -debugcon file:ckos.log