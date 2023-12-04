#include "aoc.h"

void solver(void) {
    u64 total = 0;
    String digits = { .length = 31, .data = (u8[32]){0} };

    PARSE_INPUT("\n") {
        u64 number = 0;

        while (!pDone()) {
            u8 c = pChar();

            number *= 5;
            if      (c == '2') { number += 2; }
            else if (c == '1') { number += 1; }
            else if (c == '-') { number -= 1; }
            else if (c == '=') { number -= 2; }
        }

        total += number;
    }

    u64 rem = total;
    for (us at = digits.length - 1; rem > 0; --at) {
        digits.data[at] = '0' + rem % 5;
        rem /= 5;
    }

    while (digits.data[0] == 0) { --digits.length; ++digits.data; }

    for (us at = digits.length - 1; at > 0; --at) {
        if (digits.data[at] > '2') {
            ++digits.data[at - 1];
            digits.data[at] = (digits.data[at] == '3' ? '=' : '-');
        }
    }

    solve("{S}", digits);
    solve("{s}", "");
}
