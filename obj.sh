#!/bin/bash
file="${1##*/}"
ext="${file##*.}"
file="${file%.*}"
objcopy -I binary -B arm -O elf32-littlearm                        \
        --redefine-sym _binary____"$file"_"$ext"_start="$file"    \
        --redefine-sym _binary____"$file"_"$ext"_end="$file"End    \
        --redefine-sym _binary____"$file"_"$ext"_size="$file"Size  \
        "$1" "$file".o
