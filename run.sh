#!/bin/bash

./build.sh
qemu-system-x86_64 \
    -m 128 \
    -no-reboot \
    -cdrom ckos.iso \
    -nographic \
    -debugcon file:ckos.log