#include "../aoc.h"

void solver(void) {
    u64 table_1[3][3] = {
        /*    US: R  P  S */
        /* R */ { 3, 6, 0 },
        /* P */ { 0, 3, 6 },
        /* S */ { 6, 0, 3 },
    };

    u64 table_2[3][3] = {
        /*    US: L  D  W */
        /* R */ { 3, 1, 2, },
        /* P */ { 1, 2, 3, },
        /* S */ { 2, 3, 1, },
    };

    u64 score_1 = 0;
    u64 score_2 = 0;

    PARSE_INPUT("\n") {
        u8 col_1 = pChar() - 'A';
        pAssert(" ");
        u8 col_2 = pChar() - 'X';

        score_1 += 1 + col_2 + table_1[col_1][col_2];
        score_2 += 3 * col_2 + table_2[col_1][col_2];
    }

    solve("{u}", score_1);
    solve("{u}", score_2);
}
