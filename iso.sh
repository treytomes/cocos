#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/cocos.kernel isodir/boot/cocos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "cocos" {
	multiboot /boot/cocos.kernel
}
EOF
grub-mkrescue -o cocos.iso isodir

sudo cp cocos.iso /media/host/temp
