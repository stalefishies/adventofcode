#include "aoc.h"

#define ARR_T u8
#define ARR_N jet
#include "arr.h"

#define ARR_T us
#include "arr.h"

#define LEN (64 - 7)
typedef struct State {
    u32 n;
    u16 j;
    u8  r;
    u8  grid[LEN];
} State;

bool stateEq(State a, State b) {
    if (a.j != b.j) { return false; }
    if (a.r != b.r) { return false; }
    for (us i = 0; i < LEN; ++i) {
        if (a.grid[i] != b.grid[i]) { return false; }
    }
    return true;
}

u64 stateHash(State state) {
    return strHash((String){ state.grid, LEN })
        ^ ((u64)state.j << 48) ^ ((u64)state.r << 40);
}

#define SET_KEY  State
#define SET_EQ   stateEq
#define SET_HASH stateHash
#include "set.h"

void solver(void) {
    u8 *jet = NULL;
    PARSE_INPUT() {
        jetPush(&jet, (pChar() == '>'));
    }
    us jet_count = jetLen(jet);

    u32 rocks[5]  = { 0x0000001EU, 0x00081C08U, 0x0004041CU, 0x10101010U, 0x00001818U };
    us rock_count = 5;

    State state = { .grid[4] = 0x7F };
    set_t states = {0};

    us *height = NULL;
    arrPush(&height, 0);
    us n_0 = 0, n_1 = 0;

    for (;;) {
        ++state.n;

        u32 rock = rocks[state.r];
        state.r = (state.r + 1) % rock_count;

        // first three rock shifts
        for (us i = 0; i < 3; ++i) {
            if (jet[state.j] && !(rock & 0x01010101U)) {
                rock >>= 1;
            } else if (!jet[state.j] && !(rock & 0x40404040U)) {
                rock <<= 1;
            }
            
            state.j = (state.j + 1) % jet_count;
        }

        // place rock
        u32 space =
              ((u32)state.grid[0] << 24)
            | ((u32)state.grid[1] << 16)
            | ((u32)state.grid[2] <<  8)
            | ((u32)state.grid[3] <<  0);

        for (us at = 0;; ++at) {
            if (at + 4 > LEN) { error("length too short"); }

            if (jet[state.j] && !(rock & 0x01010101U)) {
                u32 new_rock = rock >> 1;
                if (!(new_rock & space)) { rock = new_rock; }
            } else if (!jet[state.j] && !(rock & 0x40404040U)) {
                u32 new_rock = rock << 1;
                if (!(new_rock & space)) { rock = new_rock; }
            }

            state.j = (state.j + 1) % jet_count;
            u32 new_space = (space << 8) | state.grid[at + 4];

            if (rock & new_space) {
                state.grid[at + 0] |= (rock >> 24) & 0xFF;
                state.grid[at + 1] |= (rock >> 16) & 0xFF;
                state.grid[at + 2] |= (rock >>  8) & 0xFF;
                state.grid[at + 3] |= (rock >>  0) & 0xFF;

                break;
            }

            space = new_space;
        }

        // shift to height
        arrPush(&height, height[state.n - 1]);
        while (state.grid[0] || state.grid[1] || state.grid[2] || state.grid[3]) {
            for (us i = LEN - 1; i > 0; --i) {
                state.grid[i] = state.grid[i - 1];
            }

            state.grid[0] = 0x00;
            ++height[state.n];
        }

        State prev = state;
        if (setRead(states, &prev)) {
            n_0 = prev.n;
            n_1 = state.n;

            break;
        }

        setAdd(&states, state);
    }

    for (us count = 2022;;) {
        us reps  = (count - n_0) / (n_1 - n_0);
        us extra = count - n_0 - reps * (n_1 - n_0);

        solve("{u}", height[n_0 + extra] + reps * (height[n_1] - height[n_0]));

        if (count == 2022) { count = 1000000000000; } else { break; }
    }
}
