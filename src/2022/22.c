#include "aoc.h"

typedef struct Pos { u8 i, j, d; } Pos;
u64 posValue(Pos pos) {
    return 1000 * (pos.i + 1) + 4 * (pos.j + 1) + pos.d;
}

#define SET_KEY Pos
#define SET_MAP Pos
#define SET_HASH posValue
#define SET_EQ(p, q) p.i == q.i && p.j == q.j && p.d == q.d
#include "set.h"

#define TURN_L 0xFEU
#define TURN_R 0xFFU
#define ARR_T u8
#include "arr.h"

#define DIR_R 0
#define DIR_D 1
#define DIR_L 2
#define DIR_U 3

static u8 di[4] = { 0, 1,  0, -1 };
static u8 dj[4] = { 1, 0, -1,  0 };

void addEdge(set_t *set,
    u8 from_j, u8 from_i, u8 from_d, u8 from_move,
    u8   to_j, u8   to_i, u8   to_d, u8   to_move,
    us N)
{
    Pos from = { .i = from_i, .j = from_j, .d = from_d };
    Pos   to = { .i =   to_i, .j =   to_j, .d =   to_d };

    for (us n = 0; n < N; ++n) {
        setAdd(set, from, to);

        from.i += di[from_move];
        from.j += dj[from_move];
        to.i   += di[  to_move];
        to.j   += dj[  to_move];
    }
}

void solver(void) {
    String move_str = input;
    String grid_str = strSplit(&move_str, strLiteral("\n\n"));

    u8 W = strIndex(grid_str, strLiteral("\n"));
    u8 H = strCount(grid_str, strLiteral("\n")) + 1;

    u8 *map = alloc(u8, W * H);
    for (us i = 0; i < W; ++i) {
        for (us j = 0; j < H; ++j) {
            map[i * W + j] = ' ';
        }
    }

    u8 i_init = 0;
    u8 j_init = 0;

    {
        u8 pos_i = 0;
        u8 pos_j = 0;

        PARSE_STRING(grid_str) {
            u8 c = pChar();

            if (c == '\n') {
                ++pos_i;
                pos_j = 0;
                continue;
            } else if (c != ' ') {
                if (j_init == 0) { j_init = pos_j; }
                map[pos_i * W + pos_j] = c;
            }

            ++pos_j;
        }
    }

    u8 *moves = NULL;
    PARSE_STRING(move_str) {
        while (!pDone()) {
            u8 c = pPeek();
            if (c == '\0' || c == '\n') { break; }

            if      (c == 'L') { arrPush(&moves, TURN_L); pChar(); }
            else if (c == 'R') { arrPush(&moves, TURN_R); pChar(); }
            else               { arrPush(&moves, (u8)pNum()); }
        }

        break;
    }

    set_t edges_part[2] = { {0}, {0} };

    addEdge(&edges_part[0],   0, 100, DIR_U, DIR_R,   0, 199, DIR_U, DIR_R, 50);
    addEdge(&edges_part[0],  50,   0, DIR_U, DIR_R,  50, 149, DIR_U, DIR_R, 50);
    addEdge(&edges_part[0], 100,   0, DIR_U, DIR_R, 100,  49, DIR_U, DIR_R, 50);

    addEdge(&edges_part[0],   0, 199, DIR_D, DIR_R,   0, 100, DIR_D, DIR_R, 50);
    addEdge(&edges_part[0],  50, 149, DIR_D, DIR_R,  50,   0, DIR_D, DIR_R, 50);
    addEdge(&edges_part[0], 100,  49, DIR_D, DIR_R, 100,   0, DIR_D, DIR_R, 50);

    addEdge(&edges_part[0],  50,   0, DIR_L, DIR_D, 149,   0, DIR_L, DIR_D, 50);
    addEdge(&edges_part[0],  50,  50, DIR_L, DIR_D,  99,  50, DIR_L, DIR_D, 50);
    addEdge(&edges_part[0],   0, 100, DIR_L, DIR_D,  99, 100, DIR_L, DIR_D, 50);
    addEdge(&edges_part[0],   0, 150, DIR_L, DIR_D,  49, 150, DIR_L, DIR_D, 50);

    addEdge(&edges_part[0], 149,   0, DIR_R, DIR_D,  50,   0, DIR_R, DIR_D, 50);
    addEdge(&edges_part[0],  99,  50, DIR_R, DIR_D,  50,  50, DIR_R, DIR_D, 50);
    addEdge(&edges_part[0],  99, 100, DIR_R, DIR_D,   0, 100, DIR_R, DIR_D, 50);
    addEdge(&edges_part[0],  49, 150, DIR_R, DIR_D,   0, 150, DIR_R, DIR_D, 50);

    addEdge(&edges_part[1],   0, 100, DIR_U, DIR_R,  50,  50, DIR_R, DIR_D, 50);
    addEdge(&edges_part[1],  50,   0, DIR_U, DIR_R,   0, 150, DIR_R, DIR_D, 50);
    addEdge(&edges_part[1], 100,   0, DIR_U, DIR_R,   0, 199, DIR_U, DIR_R, 50);

    addEdge(&edges_part[1],   0, 199, DIR_D, DIR_R, 100,   0, DIR_D, DIR_R, 50);
    addEdge(&edges_part[1],  50, 149, DIR_D, DIR_R,  49, 150, DIR_L, DIR_D, 50);
    addEdge(&edges_part[1], 100,  49, DIR_D, DIR_R,  99,  50, DIR_L, DIR_D, 50);

    addEdge(&edges_part[1],  50,   0, DIR_L, DIR_D,   0, 149, DIR_R, DIR_U, 50);
    addEdge(&edges_part[1],  50,  50, DIR_L, DIR_D,   0, 100, DIR_D, DIR_R, 50);
    addEdge(&edges_part[1],   0, 100, DIR_L, DIR_D,  50,  49, DIR_R, DIR_U, 50);
    addEdge(&edges_part[1],   0, 150, DIR_L, DIR_D,  50,   0, DIR_D, DIR_R, 50);

    addEdge(&edges_part[1], 149,   0, DIR_R, DIR_D,  99, 149, DIR_L, DIR_U, 50);
    addEdge(&edges_part[1],  99,  50, DIR_R, DIR_D, 100,  49, DIR_U, DIR_R, 50);
    addEdge(&edges_part[1],  99, 100, DIR_R, DIR_D, 149,  49, DIR_L, DIR_U, 50);
    addEdge(&edges_part[1],  49, 150, DIR_R, DIR_D,  50, 149, DIR_U, DIR_R, 50);

    for (us part = 0; part < 2; ++part) {
        Pos pos = { .i = i_init, .j = j_init, .d = DIR_R };
        set_t edges = edges_part[part];

        for (us n = 0; n < arrLen(moves); ++n) {
            u8 count = moves[n];
            
            if (count == TURN_L) {
                pos.d = (pos.d + 3) % 4;
            } else if (count == TURN_R) {
                pos.d = (pos.d + 1) % 4;
            } else {
                for (us k = 0; k < count; ++k) {
                    Pos new, *new_ptr = setPtr(edges, pos);

                    if (new_ptr) {
                        new = *new_ptr;
                    } else {
                        new = pos;
                        new.i += di[new.d];
                        new.j += dj[new.d];
                    }

                    if (map[new.i * W + new.j] == '#') { break; }
                    pos = new;
                }
            }
        }

        solve("{u}", posValue(pos));
    }
}
