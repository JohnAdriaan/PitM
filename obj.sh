#!/bin/bash
# This script creates a linkable object (.o) file from any binary file.
# To run it, make sure the file is in the current directory.
# Then supply the complete filename as the parameter.
# A .o file will be produced, with three symbol definitions:
# "file" is the address of the start of the binary data in the program.
# "fileEnd" is the address of one past the end of the binary data.
# "fileSize" is not the size. The address of this symbol is, instead.
arch=arm
obj=elf32-littlearm
file="${1##*/}"
ext="${file##*.}"
file="${file%.*}"
objcopy -I binary -B $arch -O $obj                              \
        --redefine-sym _binary_"$file"_"$ext"_start="$file"   \
        --redefine-sym _binary_"$file"_"$ext"_end="$file"End   \
        --redefine-sym _binary_"$file"_"$ext"_size="$file"Size  \
        "$1" "$file".o
