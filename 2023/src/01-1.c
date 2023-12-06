#include "aoc.h"

void solver(void) {
    s64 sum = 0;

    PARSE_INPUT("\n") {
        u8 first = 0;
        u8 last  = 0;

        while (!pDone()) {
            u8 c = pChar();
            if (c >= '0' && c <= '9') {
                if (first == 0) { first = c - '0'; }
                last = c - '0';
            }
        }

        sum += 10 * first + last;
    }

    solve("{d}", sum);
}
