#include "aoc.h"

#define ARR_T u8
#include "arr.h"

bool compare(u8 *l_at, u8 *r_at) {
    bool order = false;
    u64 l_up = 0;
    u64 r_up = 0;

    for (;;) {
        u8 l = (l_up > 0 ? (--l_up, ']') : *l_at++);
        u8 r = (r_up > 0 ? (--r_up, ']') : *r_at++);

        if (l == '[' && r == '[') { continue; }
        if (l == ']' && r == ']') { continue; }
        if (l == ']') { order =  true; break; }
        if (r == ']') { order = false; break; }

        if (l == '[' || r == '[') {
            while (l == '[') { l = *l_at++; ++r_up; }
            while (r == '[') { r = *r_at++; ++l_up; }
        }

        if (l == ']') { order =  true; break; }
        if (r == ']') { order = false; break; }
        if (l != r) { order = (l < r); break; }
    }

    return order;
}

void solver(void) {
    u64 part_1 = 0;

    u8 div_1[5] = { '[', '[', 2, ']', ']' };
    u8 div_2[5] = { '[', '[', 6, ']', ']' };
    us div_1_pos = 0;
    us div_2_pos = 0;

    u64 index = 1;
    PARSE_INPUT("\n\n") {
        u8 *l_arr = NULL;
        u8 *r_arr = NULL;

        u8 **ptr = &l_arr;
        for (;;) {
            if (pPeek() == '[') {
                pChar();
                arrPush(ptr, '[');
            } else if (pPeek() == ']') {
                pChar();
                arrPush(ptr, ']');

                if (pPeek() == '\n' || pDone()) {
                    if (r_arr) { break; } else { ptr = &r_arr; pChar(); }
                }
            } else if (!pAccept(",")) {
                arrPush(ptr, pNum());
                pAccept(",");
            }
        }

        if (compare(l_arr, r_arr)) { part_1 += index; }
        if (compare(l_arr, div_1)) { ++div_1_pos; }
        if (compare(r_arr, div_1)) { ++div_1_pos; }
        if (compare(l_arr, div_2)) { ++div_2_pos; }
        if (compare(r_arr, div_2)) { ++div_2_pos; }

        ++index;
    }

    solve("{u}", part_1);
    solve("{u}", (div_1_pos + 1) * (div_2_pos + 2));
}
