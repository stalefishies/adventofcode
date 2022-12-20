#include "aoc.h"

#define W 143
#define H 41

void solver(void) {
    u8 height[H][W];
    u8 start_i, start_j, end_i, end_j;

    PARSE_INPUT() {
        for (u8 i = 0; i < H; ++i) {
            for (u8 j = 0; j < W; ++j) {
                u8 c = pChar();
                if (c == 'S') {
                    start_i = i;
                    start_j = j;
                    height[i][j] = 0;
                } else if (c == 'E') {
                    end_i = i;
                    end_j = j;
                    height[i][j] = 25;
                } else {
                    height[i][j] = c - 'a';
                }
            }
            pAccept("\n");
        }
    }

    u16 dist[H][W];
    for (us i = 0; i < H; ++i)
    for (us j = 0; j < W; ++j) {
        dist[i][j] = 65535;
    }

    dist[end_i][end_j] = 0;
    for (;;) {
        bool changed = false;

        for (u8 i = 0; i < H; ++i)
        for (u8 j = 0; j < W; ++j) {
            if (dist[i][j] == 65535) { continue; }

            u8 di[4] = { 1, -1, 0, 0 };
            u8 dj[4] = { 0, 0, 1, -1 };

            for (us d = 0; d < 4; ++d) {
                u8 y = i + di[d];
                u8 x = j + dj[d];

                if (y >= H || x >= W
                    || height[y][x] + 1 < height[i][j]
                    || dist[y][x] <= dist[i][j] + 1)
                { continue; }

                dist[y][x] = dist[i][j] + 1;
                changed = true;
            }
        }

        if (!changed) { break; }
    }
    
    solve("{u16}", dist[start_i][start_j]);

    u16 min_dist = dist[start_i][start_j];
    for (u8 i = 0; i < H; ++i)
    for (u8 j = 0; j < W; ++j) {
        if (height[i][j] != 0) { continue; }
        if (dist[i][j] < min_dist) { min_dist = dist[i][j]; }
    }

    solve("{u16}", min_dist);
}
