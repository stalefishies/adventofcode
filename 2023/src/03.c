#include "aoc.h"

typedef struct Part {
    u16 value;
    u8 c, j, i;
} Part;

#define ARR_T Part
#define ARR_N part
#include "arr.h"

#define W 140
#define H 140

void solver(void) {
    Part *parts = NULL;
    PARSE_STRING(input) {
        s64 left_index = pGet().data - input.data;
        s64 x0 = left_index % (W + 1);
        s64 y0 = left_index / (W + 1);

        if (pPeek() >= '0' && pPeek() <= '9') {
            s64 value = pNum();
            if (value > 0) {
                s64 x1 = (pGet().data - input.data) % (W + 1);
                s64 y1 = y0 + 1;

                bool done = false;
                for (s64 i = y0 - 1; i < y1 + 1; ++i) {
                    if (i < 0 || i >= H) { continue; }
                    for (s64 j = x0 - 1; j < x1 + 1; ++j) {
                        if (j < 0 || j >= W) { continue; }

                        u8 c = input.data[i * (W + 1) + j];
                        if (c != '.' && (c < '0' || c > '9')) {
                            Part part = { value, c, j, i };
                            partPush(&parts, part);
                            done = true;
                            break;
                        }
                    }

                    if (done) { break; }
                }
            }
        }

        pChar();
    }

    s64 count = partLen(parts);
    s64 sum_parts = 0;
    s64 sum_gears = 0;

    for (s64 i = 0; i < count; ++i) {
        Part part = parts[i];
        sum_parts += part.value;

        if (part.c != '*') { continue; }
        for (s64 j = i + 1; j < count; ++j) {
            Part other = parts[j];
            if (other.c != '*' || other.i != part.i || other.j != part.j)
                { continue; }

            sum_gears += part.value * other.value;
        }
    }

    solve("{d}", sum_parts);
    solve("{d}", sum_gears);
}
