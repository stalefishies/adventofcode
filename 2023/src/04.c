#include "aoc.h"

#define CARD_COUNT 202
#define DRAW_COUNT 10
#define HAVE_COUNT 25

void solver(void) {
    s64 matches[CARD_COUNT];
    PARSE_INPUT("\n") {
        pAssert("Card");
        while (pAccept(" "));
        s64 card = pNum() - 1;
        pAssert(":");

        u8 draw[DRAW_COUNT];
        u8 have[HAVE_COUNT];

        for (int i = 0; i < DRAW_COUNT; ++i) {
            while (pAccept(" "));
            draw[i] = pNum();
        }
        
        pAssert(" |");

        for (int i = 0; i < HAVE_COUNT; ++i) {
            while (pAccept(" "));
            have[i] = pNum();
        }

        matches[card] = 0;
        for (int i = 0; i < HAVE_COUNT; ++i) {
            for (int j = 0; j < DRAW_COUNT; ++j) {
                if (have[i] == draw[j]) {
                    matches[card]++;
                    break;
                }
            }

        }
    }

    s64 points = 0;
    s64 total_count = 0;

    s64 counts[CARD_COUNT];
    for (s64 i = 0; i < CARD_COUNT; ++i) { counts[i] = 1; }

    for (s64 i = 0; i < CARD_COUNT; ++i) {
        total_count += counts[i];
        if (matches[i] == 0) { continue; }
        
        // Part 1
        points += (1 << (matches[i] - 1));

        // Part 2
        for (s64 j = 1; j <= matches[i]; ++j) { counts[i + j] += counts[i]; }
    }

    solve("{d}", points);
    solve("{d}", total_count);
}
