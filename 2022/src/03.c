#include "aoc.h"

void solver(void) {
    u64 sum_one = 0;
    u64 sum_two = 0;

    PARSE_INPUT() {
        us index = 0;
        u64 group[3] = {0};

        PARSE_SPLIT("\n") {
            us length = pLen();
            u64 flags_one = 0;
            u64 flags_two = 0;

            for (us i = 0; i < length / 2; ++i) {
                u8 c = pChar();
                u8 p = 1 + (c >= 'a' ? c - 'a' : 26 + c - 'A');
                flags_one |= 1ULL << p;
            }

            for (us i = 0; i < length / 2; ++i) {
                u8 c = pChar();
                u8 p = 1 + (c >= 'a' ? c - 'a' : 26 + c - 'A');

                if ((flags_one & (1ULL << p)) && !(flags_two & (1ULL << p))) { sum_one += p; }
                flags_two |= 1ULL << p;
            }

            group[index++] = (flags_one | flags_two);
            if (index == 3) {
                u64 flag = (group[0] & group[1] & group[2]) >> 1;

                u64 p = 0;
                while (flag) { flag >>= 1; ++p; }
                sum_two += p;

                group[0] = group[1] = group[2] = index = 0;
            }
        }
    }

    solve("{u}", sum_one);
    solve("{u}", sum_two);
}
