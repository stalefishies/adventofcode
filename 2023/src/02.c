#include "aoc.h"

void solver(void) {
    s64 game_sum = 0;
    s64 power_sum = 0;

    PARSE_INPUT("\n") {
        pAssert("Game ");
        s64 game = pNum();
        pAssert(": ");

        s64 max_r = 0;
        s64 max_g = 0;
        s64 max_b = 0;

        while (!pDone()) {
            s64 count = pNum();
            pAssert(" ");

            if (pAccept("red")) {
                max_r = (count > max_r ? count : max_r);
            } else if (pAccept("green")) {
                max_g = (count > max_g ? count : max_g);
            } else if (pAccept("blue")) {
                max_b = (count > max_b ? count : max_b);
            }

            pAccept(", ");
            pAccept("; ");
        }

        if (max_r <= 12 && max_g <= 13 && max_b <= 14) {
            game_sum += game;
        }

        s64 power = max_r * max_g * max_b;
        power_sum += power;
    }

    solve("{d}", game_sum);
    solve("{d}", power_sum);
}
