#include "aoc.h"

typedef struct Pos { u32 i, j; } Pos;

typedef struct Move {
    Pos at;
    Pos to;
    u8 valid;
} Move;

#define ARR_T Move
#define ARR_CMP(m, n) (m.to.i == n.to.i ? m.to.j - n.to.j : m.to.i - n.to.i)
#include "arr.h"

void solver(void) {
//    input = strLiteral(
//"....#..\n"
//"..###.#\n"
//"#...#.#\n"
//".#...##\n"
//"#.###..\n"
//"##.#.##\n"
//".#..#..\n"
//    );

    us border = 100;
    us stride = strIndex(input, strLiteral("\n")) + 2 * border;

    u8 *grid = alloc(u8,  stride * stride);
    for (us i = 0; i < stride; ++i) {
        for (us j = 0; j < stride; ++j) {
            grid[i * stride + j] = 0;
        }
    }

    Move *moves = NULL;
    PARSE_INPUT() {
        Pos pos = { .i = border, .j = border };

        while (!pDone()) {
            u8 c = pChar();
            if (c == '\n') { ++pos.i; pos.j = border; continue; }

            if (c == '#') {
                arrPush(&moves, (Move){ .at = pos, .to = (Pos){0}, .valid = false });
            }

            ++pos.j;
        }
    }

    us count = arrLen(moves);
    for (us index = 0; index < count; ++index) {
        grid[moves[index].at.i * stride + moves[index].at.j] = 1;
    }

    u32 di[4] = { -1,  1,  0,  0 };
    u32 dj[4] = {  0,  0, -1,  1 };

    for (us round = 0; round < 1000; ++round) {
        for (us index = 0; index < count; ++index) {
            Pos at = moves[index].at;

            u8 nn_NW = grid[(at.i - 1) * stride + at.j - 1];
            u8 nn_N  = grid[(at.i - 1) * stride + at.j    ];
            u8 nn_NE = grid[(at.i - 1) * stride + at.j + 1];
            u8 nn_W  = grid[(at.i    ) * stride + at.j - 1];
            u8 nn_E  = grid[(at.i    ) * stride + at.j + 1];
            u8 nn_SW = grid[(at.i + 1) * stride + at.j - 1];
            u8 nn_S  = grid[(at.i + 1) * stride + at.j    ];
            u8 nn_SE = grid[(at.i + 1) * stride + at.j + 1];

            u8 blocked[4] = {
                nn_NW || nn_N || nn_NE,
                nn_SW || nn_S || nn_SE,
                nn_NW || nn_W || nn_SW,
                nn_NE || nn_E || nn_SE
            };

            moves[index].valid = false;
            if (blocked[0] || blocked[1] || blocked[2] || blocked[3]) {
                for (us n = 0; n < 4; ++n) {
                    us d = (round + n) % 4;

                    if (!blocked[d]) {
                        moves[index].to = (Pos){ .i = at.i + di[d], .j = at.j + dj[d] };
                        moves[index].valid = true;

                        break;
                    }
                }
            }
        }

        bool any_moved = false;
        for (us index = 0; index < count; ++index) {
            if (!moves[index].valid) { continue; }

            for (us other = index + 1; other < count; ++other) {
                if (!moves[other].valid) { continue; }

                if (moves[index].to.i == moves[other].to.i
                    && moves[index].to.j == moves[other].to.j)
                {
                    moves[index].valid = false;
                    moves[other].valid = false;
                }
            }

            if (!moves[index].valid) { continue; }
            any_moved = true;

            grid[moves[index].at.i * stride + moves[index].at.j] = 0;

            moves[index].at = moves[index].to;
            moves[index].to = (Pos){0};
            moves[index].valid = false;

            grid[moves[index].at.i * stride + moves[index].at.j] = 1;
        }

        if (round == 9) {
            u32 i_min = stride, i_max = 0, j_min = stride, j_max = 0;

            for (us index = 0; index < count; ++index) {
                Pos at = moves[index].at;

                if (at.i < i_min) { i_min = at.i; }
                if (at.i > i_max) { i_max = at.i; }
                if (at.j < j_min) { j_min = at.j; }
                if (at.j > j_max) { j_max = at.j; }
            }

            solve("{u}", (i_max - i_min + 1) * (j_max - j_min + 1) - count);
        }

        if (!any_moved) {
            solve("{u}", round + 1);
            break;
        }
    }
}
