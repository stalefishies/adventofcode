#include "aoc.h"

typedef struct Monkey {
    u8 index;
    u8 count;
    bool square;
    u8 mult;
    u8 add;
    u8 test;
    u8 dest[2];
    u64 insp;
    u64 items[36];
} Monkey;

void solver(void) {
    String input = readInput();
    Monkey monkey_init[8];
    
    PARSE_INPUT("\n\n") {
        pAssert("Monkey ");
        us index = pNum();
        Monkey *m = &monkey_init[index];
        m->index = index;

        pAssert(":\n  Starting items: ");
        m->count = 0;
        m->insp = 0;
        for (;;) {
            m->items[m->count++] = pNum();
            if (!pAccept(", ")) { break; }
        }

        pAssert("\n  Operation: new = old ");
        if (pAccept("* old")) {
            m->square = true;
            m->mult   = 1;
            m->add    = 0;
        } else if (pAccept("* ")) {
            m->square = false;
            m->mult   = pNum();
            m->add    = 0;
        } else {
            pAssert("+ ");
            m->square = false;
            m->mult   = 1;
            m->add    = pNum();
        }

        pAssert("\n  Test: divisible by ");
        m->test = pNum();
        pAssert("\n    If true: throw to monkey ");
        m->dest[0] = pNum();
        pAssert("\n    If false: throw to monkey ");
        m->dest[1] = pNum();
    }
    
    u64 modulus = 1;
    for (us i = 0; i < 8; ++i) { modulus *= monkey_init[i].test; }

    u64 rounds = 20;
    bool div = true;

    for (us part = 1; part <= 2; ++part) {
        Monkey monkey[8];
        for (us i = 0; i < 8; ++i) { monkey[i] = monkey_init[i]; }

        for (us round = 1; round <= rounds; ++round) {
            for (us index = 0; index < 8; ++index) {
                Monkey *m = &monkey[index];

                for (us i = 0; i < m->count; ++i) {
                    ++m->insp;
                    u64 item = m->items[i];
                    if (m->square) { item *= item; }
                    item = (item * m->mult + m->add) % modulus;
                    if (div) { item /= 3; }

                    u8 dest = m->dest[!!(item % m->test)];
                    monkey[dest].items[monkey[dest].count++] = item;
                }

                m->count = 0;
            }
        }

        u64 max_insp[2] = {0};
        for (us i = 0; i < 8; ++i) {
            if (monkey[i].insp > max_insp[0]) {
                max_insp[1] = max_insp[0];
                max_insp[0] = monkey[i].insp;
            } else if (monkey[i].insp > max_insp[1]) {
                max_insp[1] = monkey[i].insp;
            }
        }

        solve("{u}", max_insp[0] * max_insp[1]);

        if (part == 2) { break; }
        rounds = 10000;
        div    = false;
    }
}
