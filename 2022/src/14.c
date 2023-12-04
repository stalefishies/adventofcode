#include "aoc.h"

typedef struct Pos { s32 i, j; } Pos;
#define SET_KEY Pos
#define SET_EQ(p, q) p.i == q.i && p.j == q.j
#define SET_HASH(p) ((u64)p.i << 32) | (u64)p.j
#include "set.h"

void solver(void) {
    set_t filled = {0};
    s32 i_max = 0;

    PARSE_INPUT("\n") {
        s32 j0 = pNum();
        pAssert(",");
        s32 i0 = pNum();
        if (i0 + 1 > i_max) { i_max = i0 + 1; }

        while (pAccept(" -> ")) {
            s32 j1 = pNum();
            pAssert(",");
            s32 i1 = pNum();

            s32 i_lo = i0 < i1 ? i0 : i1;
            s32 i_hi = i0 < i1 ? i1 : i0;
            s32 j_lo = j0 < j1 ? j0 : j1;
            s32 j_hi = j0 < j1 ? j1 : j0;
            
            setAdd(&filled, (Pos){ i_lo, j_lo });
            for (s32 i = i_lo, j = j_lo; i < i_hi || j < j_hi;) {
                if (i < i_hi) { ++i; }
                if (j < j_hi) { ++j; }
                setAdd(&filled, (Pos){ i, j });
            }

            i0 = i1;
            j0 = j1;
            if (i0 + 1 > i_max) { i_max = i0 + 1; }
        }
    }

    us rock_count = filled.num;
    bool part_one = false;

    for (s32 i = 0, j = 500;;) {
        if (i < i_max) {
            if (!setHas(filled, (Pos){ i + 1, j })) {
                ++i; continue;
            } else if (!setHas(filled, (Pos){ i + 1, j - 1 })) {
                ++i; --j; continue;
            } else if (!setHas(filled, (Pos){ i + 1, j + 1 })) {
                ++i; ++j; continue;
            }
        }

        if (!part_one && i == i_max) {
            solve("{u}", filled.num - rock_count);
            part_one = true;
        }

        setAdd(&filled, (Pos){ i, j });
        if (i == 0 && j == 500) { break; }
        i = 0; j = 500;
    }

    solve("{u}", filled.num - rock_count);
}
