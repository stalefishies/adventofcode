#include "aoc.h"

#define ARR_T s64
#include "arr.h"

void solver(void) {
    String input = readInput();
    s64 *instr = NULL;

    PARSE_INPUT("\n") {
        if (pAccept("addx ")) {
            arrPush(&instr, pNum());
        } else {
            pAssert("noop");
            arrPush(&instr, 0);
        }
    }

    s64 cycle  = 1;
    s64 X      = 1;
    s64 part_1 = 0;

    u8 screen[240];
    for (us i = 0; i < arrLen(instr); ++i) {
        s64 add = instr[i];
        for (s64 n = cycle; n <= cycle + (add != 0); ++n) {
            if ((n - 20) % 40 == 0) {
                part_1 += X * n;
            }

            s64 x = (n - 1) % 40;
            if (x == X - 1 || x == X || x == X + 1) {
                screen[n - 1] = '#';
            } else {
                screen[n - 1] = '.';
            }
        }

        X += add;
        cycle += 1 + (add != 0);
    }

    /*
    print("{}", (String){ screen +   0, 40 });
    print("{}", (String){ screen +  40, 40 });
    print("{}", (String){ screen +  80, 40 });
    print("{}", (String){ screen + 120, 40 });
    print("{}", (String){ screen + 160, 40 });
    print("{}", (String){ screen + 200, 40 });
    */

    solve("{d}", part_1);
    solve("{}", strLiteral("FCJAPJRE"));
}
