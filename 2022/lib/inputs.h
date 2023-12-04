#ifndef INPUTS_HGUARD
#define INPUTS_HGUARD

#include "common.h"

EXPORT String readInputFile(int day);

#if (YEAR >= 2015) && (DAY > 0)
#   define readInput() readInputFile(DAY)
#else
#   define readInput() (String){ NULL, 0 }
#endif

#endif /* INPUTS_HGUARD */

#ifdef  AOC_IMPLEMENTATION
#ifndef INPUTS_IMPLEMENTED
#define INPUTS_IMPLEMENTED

#include <stdio.h>
#include "memory.h"

String readInputFile(int day) {
    char filename[16] = "../input/XX.txt";
    filename[ 9] = '0' + (day / 10) % 10;
    filename[10] = '0' + (day     ) % 10;

    String file = { NULL, 0 };
    FILE *handle = fopen(filename, "r");
    if (!handle) { error("Could not open input file {s}", filename); }

    fseek(handle, 0, SEEK_END);
    file.length = ftell(handle);
    fseek(handle, 0, SEEK_SET);
    if (!file.length) { error("Could not get input file length"); }

    file.data = alloc(u8, file.length);
    size_t read = fread(file.data, file.length, 1, handle);
    if (read != 1) { error("Could not read input file data"); }

    return file;
}

#endif /* INPUTS_IMPLEMENTED */
#endif /* AOC_IMPLEMENTATION */
