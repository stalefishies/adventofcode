#include "aoc.h"

#define W 99
#define H 99

void solver(void) {
    u8 forest[H][W];

    PARSE_INPUT() {
        for (us i = 0; i < H; ++i) {
            for (us j = 0; j < W; ++j) {
                forest[i][j] = pChar() - '0';
            }
            pAssert("\n");
        }
    }

    u64 visible = 2 * W + 2 * H - 4;
    u64 scenic  = 0;
    for (us i0 = 1; i0 < H - 1; ++i0)
    for (us j0 = 1; j0 < W - 1; ++j0) {
        us h = forest[i0][j0];

        bool visible_u = true;
        u64  scenic_u  = 0;
        for (us i = i0 - 1; i < H; --i) {
            ++scenic_u;
            if (forest[i][j0] >= h) { visible_u = false; break; }
        }

        bool visible_d = true;
        u64  scenic_d  = 0;
        for (us i = i0 + 1; i < H; ++i) {
            ++scenic_d;
            if (forest[i][j0] >= h) { visible_d = false; break; }
        }

        bool visible_l = true;
        u64  scenic_l  = 0;
        for (us j = j0 - 1; j < W; --j) {
            ++scenic_l;
            if (forest[i0][j] >= h) { visible_l = false; break; }
        }

        bool visible_r = true;
        u64  scenic_r  = 0;
        for (us j = j0 + 1; j < W; ++j) {
            ++scenic_r;
            if (forest[i0][j] >= h) { visible_r = false; break; }
        }

        visible += (visible_u || visible_d || visible_l || visible_r);
        u64 scenic_this = scenic_u * scenic_d * scenic_l * scenic_r;
        if (scenic_this > scenic) { scenic = scenic_this; }
    }

    solve("{u}", visible);
    solve("{u}", scenic);
}
