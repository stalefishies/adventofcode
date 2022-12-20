#ifndef INPUTS_HGUARD
#define INPUTS_HGUARD

#include "common.h"

EXPORT String readInputFile(int year, int day);

#if (YEAR >= 2015) && (DAY > 0)
#   define readInput() readInputFile(YEAR, DAY)
#else
#   define readInput() (String){ NULL, 0 }
#endif

#endif /* INPUTS_HGUARD */

#ifdef  AOC_IMPLEMENTATION
#ifndef INPUTS_IMPLEMENTED
#define INPUTS_IMPLEMENTED

#include <stdio.h>
#include "memory.h"

String readInputFile(int year, int day) {
    char filename[22] = "../inputs/XXXX/XX.txt";
    filename[10] = '0' + (year / 1000) % 10;
    filename[11] = '0' + (year / 100 ) % 10;
    filename[12] = '0' + (year / 10  ) % 10;
    filename[13] = '0' + (year       ) % 10;
    filename[15] = '0' + (day  / 10  ) % 10;
    filename[16] = '0' + (day        ) % 10;

    String file = { NULL, 0 };
    FILE *handle = fopen(filename, "r");

    if (handle) {
        fseek(handle, 0, SEEK_END);
        file.length = ftell(handle);
        fseek(handle, 0, SEEK_SET);
        if (!file.length) { error("Could not get file length"); }

        file.data = alloc(u8, file.length);
        size_t read = fread(file.data, file.length, 1, handle);
        if (read != 1) { error("Could not read file data"); }
    }

    return file;
}

#endif /* INPUTS_IMPLEMENTED */
#endif /* AOC_IMPLEMENTATION */
