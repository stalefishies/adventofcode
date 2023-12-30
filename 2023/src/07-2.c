#include "aoc.h"

typedef struct Hand { u32 cmp, bid; } Hand;
#define ARR_T Hand
#define ARR_N hand
#define ARR_CMP_AS(h) h.cmp
#include "arr.h"

void solver(void) {
    Hand *hands = NULL;

    PARSE_INPUT("\n") {
        Hand hand = {0};
        u8 mult[15] = {0};
        u8 count[5] = {0};
        u8 cards[5] = {0};

        for (us i = 0; i < 5; ++i) {
            u8 card = pChar();
            if      (card == 'T') { card = 10; }
            else if (card == 'J') { card =  1; }
            else if (card == 'Q') { card = 12; }
            else if (card == 'K') { card = 13; }
            else if (card == 'A') { card = 14; }
            else { card -= '0'; }

            cards[i] = card;
            ++mult[card];
        }

        for (s64 i = 14; i >= 2; --i) {
            if (mult[i] > 0) { ++count[5 - mult[i]]; }
        }

        if (mult[1] == 5) {
            count[0] = 1;
            count[4] = 0;
        } else for (s64 i = 0; i < 5; ++i) {
            if (count[i] > 0) {
                --count[i];
                ++count[i - mult[1]];
                break;
            }
        }

        hand.cmp = ((u32)count[0] << 29) 
                 | ((u32)count[1] << 26) 
                 | ((u32)count[2] << 23) 
                 | ((u32)count[3] << 20) 
                 | ((u32)cards[0] << 16) 
                 | ((u32)cards[1] << 12) 
                 | ((u32)cards[2] <<  8) 
                 | ((u32)cards[3] <<  4) 
                 | ((u32)cards[4] <<  0);

        pAssert(" ");
        hand.bid = pNum();

        handPush(&hands, hand);
    }

    handSortInc(hands);

    s64 winnings = 0;
    for (us i = 0; i < handLen(hands); ++i) {
        Hand hand = hands[i];
        winnings += hand.bid * (i + 1);
    }

    solve("{d}", winnings);
}
