#include "aoc.h"

typedef struct Pos { s32 i, j; } Pos;

#define SET_KEY Pos
#define SET_EQ(p, q) p.i == q.i && p.j == q.j
#define SET_HASH(p) (u64)(1e8 + p.i * 1e4 + p.j)
#include "set.h"

void solver(void) {
    String input = readInput();

    Pos pos[10];
    set_t set_1 = {0};
    set_t set_9 = {0};

    for (us n = 0; n < 10; ++n) { pos[n] = (Pos){ 0, 0 }; }
    setAdd(&set_1, pos[1]);
    setAdd(&set_9, pos[9]);

    PARSE_INPUT("\n") {
        u8 dir = pChar();
        pAssert(" ");
        u8 count = pNum();

        for (us n = 0; n < count; ++n) {
            if      (dir == 'U') { --pos[0].i; }
            else if (dir == 'D') { ++pos[0].i; }
            else if (dir == 'L') { --pos[0].j; }
            else if (dir == 'R') { ++pos[0].j; }

            for (us n = 1; n < 10; ++n) {
                if (pos[n].i < pos[n - 1].i - 1 || pos[n].i > pos[n - 1].i + 1
                        || pos[n].j < pos[n - 1].j - 1 || pos[n].j > pos[n - 1].j + 1)
                {
                    if      (pos[n].i < pos[n - 1].i) { ++pos[n].i; }
                    else if (pos[n].i > pos[n - 1].i) { --pos[n].i; }
                    if      (pos[n].j < pos[n - 1].j) { ++pos[n].j; }
                    else if (pos[n].j > pos[n - 1].j) { --pos[n].j; }
                }
            }

            setAdd(&set_1, pos[1]);
            setAdd(&set_9, pos[9]);
        }
    }

    solve("{u}", set_1.num);
    solve("{u}", set_9.num);
}
