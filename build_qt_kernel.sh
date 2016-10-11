#!/bin/bash

export ARCH=arm
export CROSS_COMPILE=$(pwd)/../prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-

cp config_for_iTOPiMX6_linux .config

make zImage -j4

make uImage -j4

make modules

