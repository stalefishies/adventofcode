#include "../aoc.h"

#define MAX 60

void solver(void) {
    u8 size[10] = {0};
    u8 stack_1[10][MAX] = {0};
    u8 stack_2[10][MAX] = {0};

    PARSE_INPUT() {
        String init = pGet();

        us init_height = 0;
        while (!pAccept(" 1")) {
            pSplit("\n");
            ++init_height;
        }

        pSet(init);
        for (us h = init_height - 1; h < init_height; --h) {
            for (us i = 1; i <= 9; ++i) {
                if (pAccept("[")) {
                    u8 c = pChar();
                    pAssert("]");

                    stack_1[i][h] = c;
                    size[i] = (h + 1 > size[i] ? h + 1 : size[i]);
                } else {
                    pAssert("   ");
                }

                if (i < 9) { pAssert(" "); }
            }
            pAssert("\n");
        }

        for (us i = 1; i <= 9; ++i) {
            for (us j = 0; j < size[i]; ++j) {
                stack_2[i][j] = stack_1[i][j];
            }
        }

        pAssert(" 1   2   3   4   5   6   7   8   9 \n\n");

        PARSE_SPLIT("\n") {
            pAssert("move ");
            us count = pNum();
            pAssert(" from ");
            us from = pNum();
            pAssert(" to ");
            us to = pNum();

            size[from] -= count;
            for (us n = 0; n < count; ++n) {
                us m = count - 1 - n;
                stack_1[to][size[to] + n] = stack_1[from][size[from] + m];
                stack_2[to][size[to] + n] = stack_2[from][size[from] + n];
            }
            size[to] += count;
        }
    }

    char answer_1[10] = {0};
    char answer_2[10] = {0};

    for (us i = 0; i < 9; ++i) {
        answer_1[i] = stack_1[i + 1][size[i + 1] - 1];
        answer_2[i] = stack_2[i + 1][size[i + 1] - 1];
    }

    solve(answer_1);
    solve(answer_2);
}
