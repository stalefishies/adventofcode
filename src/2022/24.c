#include "aoc.h"

#define H 25
#define W 120
typedef struct Grid { u8 at[H][W]; } Grid;
#define GET(g, i, j) ((g)->at[((i) + H) % H][((j) + W) % W])

#define R 0x01
#define L 0x02
#define D 0x04
#define U 0x08
#define X 0x80

void stepGrid(Grid **curr, Grid **next, u64 *time) {
    for (us i = 0; i < H; ++i) {
        for (us j = 0; j < W; ++j) {
            u8 l = (*curr)->at[i][(j + W - 1) % W];
            u8 r = (*curr)->at[i][(j     + 1) % W];
            u8 u = (*curr)->at[(i + H - 1) % H][j];
            u8 d = (*curr)->at[(i     + 1) % H][j];

            (*next)->at[i][j] = 0;
            if (l & R) { (*next)->at[i][j] |= R; }
            if (r & L) { (*next)->at[i][j] |= L; }
            if (u & D) { (*next)->at[i][j] |= D; }
            if (d & U) { (*next)->at[i][j] |= U; }

            if (!(*next)->at[i][j] && ((*curr)->at[i][j] == X
                || (j > 0     && l == X)
                || (j < W - 1 && r == X)
                || (i > 0     && u == X)
                || (i < H - 1 && d == X)))
            { (*next)->at[i][j] = X; }
        }
    }

    Grid *tmp = *curr;
    *curr = *next;
    *next = tmp;
    ++(*time);
}

void solver(void) {
    Grid grid[2] = {{0}, {0}};

    PARSE_INPUT() {
        pSplit("\n");

        for (us i = 0; i < H; ++i) {
            pAssert("#");

            for (us j = 0; j < W; ++j) {
                u8 c = pChar();
                if      (c == '>') { grid[0].at[i][j] = R; }
                else if (c == '<') { grid[0].at[i][j] = L; }
                else if (c == 'v') { grid[0].at[i][j] = D; }
                else if (c == '^') { grid[0].at[i][j] = U; }
                else               { grid[0].at[i][j] = 0; }
            }

            pAssert("#\n");
        }

        pLen() = 0;
    }

    Grid *curr = &grid[0];
    Grid *next = &grid[1];

    u64 time = 0;
    stepGrid(&curr, &next, &time);

    for (;;) {
        if (curr->at[0][0] == 0) { curr->at[0][0] = X; }
        stepGrid(&curr, &next, &time);
        if (curr->at[H - 1][W - 1] == X) { break; }
    }
    stepGrid(&curr, &next, &time);
    solve("{u}", time);

    for (us i = 0; i < H; ++i) {
        for (us j = 0; j < W; ++j) {
            if (curr->at[i][j] == X) { curr->at[i][j] = 0; }
        }
    }
    stepGrid(&curr, &next, &time);

    for (;;) {
        if (curr->at[H - 1][W - 1] == 0) { curr->at[H - 1][W - 1] = X; }
        stepGrid(&curr, &next, &time);
        if (curr->at[0][0] == X) { break; }
    }
    stepGrid(&curr, &next, &time);

    for (us i = 0; i < H; ++i) {
        for (us j = 0; j < W; ++j) {
            if (curr->at[i][j] == X) { curr->at[i][j] = 0; }
        }
    }
    stepGrid(&curr, &next, &time);

    for (;;) {
        if (curr->at[0][0] == 0) { curr->at[0][0] = X; }
        stepGrid(&curr, &next, &time);
        if (curr->at[H - 1][W - 1] == X) { break; }
    }
    stepGrid(&curr, &next, &time);
    solve("{u}", time);
}
