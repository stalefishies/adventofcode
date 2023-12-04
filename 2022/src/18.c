#include "aoc.h"

#define N 22
typedef u8 Field[N][N][N];

u64 surface(Field field) {
    u64 count = 0;
    
    for (us i = 0; i < N; ++i)
    for (us j = 0; j < N; ++j)
    for (us k = 0; k < N; ++k) {
        if (!field[i][j][k]) { continue; }

        if (i ==     0 || !field[i - 1][j][k]) { ++count; }
        if (i == N - 1 || !field[i + 1][j][k]) { ++count; }
        if (j ==     0 || !field[i][j - 1][k]) { ++count; }
        if (j == N - 1 || !field[i][j + 1][k]) { ++count; }
        if (k ==     0 || !field[i][j][k - 1]) { ++count; }
        if (k == N - 1 || !field[i][j][k + 1]) { ++count; }
    }

    return count;
}

void solver(void) {
    Field lava = {0};

    PARSE_INPUT("\n") {
        u64 x = pNum();
        pAssert(",");
        u64 y = pNum();
        pAssert(",");
        u64 z = pNum();

        lava[x][y][z] = 1;
    }

    u64 lava_surface = surface(lava);

    Field exterior = {0};
    for (us i = 0; i < N; ++i)
    for (us j = 0; j < N; ++j)
    for (us k = 0; k < N; ++k) {
        exterior[i][j][k] = (lava[i][j][k] == 0 &&
            (i == 0 || i == N - 1 || j == 0 || j == N - 1 || k == 0 || k == N - 1));
    }

    for (bool changed = true; changed;) {
        changed = false;

        for (us i = 0; i < N; ++i)
        for (us j = 0; j < N; ++j)
        for (us k = 0; k < N; ++k) {
            if (!exterior[i][j][k]) { continue; }

            us di[6] = { +1, -1,  0,  0,  0,  0 };
            us dj[6] = {  0,  0, +1, -1,  0,  0 };
            us dk[6] = {  0,  0,  0,  0, +1, -1 };

            for (us n = 0; n < 6; ++n) {
                us new_i = i + di[n];
                us new_j = j + dj[n];
                us new_k = k + dk[n];

                if (new_i >= N || new_j >= N || new_k >= N) { continue; }
                
                if (!exterior[new_i][new_j][new_k] && !lava[new_i][new_j][new_k]) {
                    exterior[new_i][new_j][new_k] = 1;
                    changed = true;
                }
            }
        }
    }

    Field interior = {0};
    for (us i = 0; i < N; ++i)
    for (us j = 0; j < N; ++j)
    for (us k = 0; k < N; ++k) {
        interior[i][j][k] = (!lava[i][j][k] && !exterior[i][j][k]);
    }

    u64 ext_surface = lava_surface - surface(interior);

    solve("{u}", lava_surface);
    solve("{u}", ext_surface);
}
