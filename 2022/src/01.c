#include "aoc.h"

void solver(void) {
    s64 max[3] = {0};

    PARSE_INPUT("\n\n") {
        s64 current = 0;

        PARSE_SPLIT("\n") {
            current += pNum();
        }

        if (current > max[2]) {
            max[2] = current;
            if (max[2] > max[1]) { s64 tmp = max[1]; max[1] = max[2]; max[2] = tmp; }
            if (max[1] > max[0]) { s64 tmp = max[0]; max[0] = max[1]; max[1] = tmp; }
        }
    }

    solve("{u}", max[0]);
    solve("{u}", max[0] + max[1] + max[2]);
}
