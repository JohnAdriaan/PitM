#!/bin/bash
arch=arm
obj=ef32-littlearm
file="${1##*/}"
ext="${file##*.}"
file="${file%.*}"
objcopy -I binary -B $arch -O $obj                              \
        --redefine-sym _binary____"$file"_"$ext"_start="$file"   \
        --redefine-sym _binary____"$file"_"$ext"_end="$file"End   \
        --redefine-sym _binary____"$file"_"$ext"_size="$file"Size  \
        "$1" "$file".o
