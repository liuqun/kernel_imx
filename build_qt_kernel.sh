#!/bin/bash

export ARCH=arm
export CROSS_COMPILE=$(pwd)/../prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-
cp arch/arm/configs/topeet_imx6_linux_supper_defconfig .config
make zImage -j4

make uImage -j4

make modules

