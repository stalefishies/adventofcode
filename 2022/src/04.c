#include "aoc.h"

void solver(void) {
    u64 full_count = 0;
    u64 part_count = 0;

    PARSE_INPUT("\n") {
        u64 lo_1 = pNum();
        pAssert("-");
        u64 hi_1 = pNum();
        pAssert(",");
        u64 lo_2 = pNum();
        pAssert("-");
        u64 hi_2 = pNum();

        if ((lo_1 <= lo_2 && hi_1 >= hi_2) || (lo_2 <= lo_1 && hi_2 >= hi_1)) { ++full_count; }
        if ((lo_1 <= hi_2 && hi_1 >= lo_2) || (lo_2 <= hi_1 && hi_2 >= lo_1)) { ++part_count; }
    }

    solve("{u}", full_count);
    solve("{u}", part_count);
}
