#include "../aoc.h"

bool unique(u8 *at, us count) {
    u32 bits = 0;

    for (us i = 0; i < count; ++i) {
        u32 bit = (1 << (*at++ - 'a'));
        if (bits & bit) { return false; }
        bits |= bit;
    }

    return true;
}

void solver(void) {
    us packet  = 0;
    us message = 0;

    PARSE_INPUT() {
        u8 *at = pGet().data;
        pChar();

        if (packet == 0 && unique(at, 4)) {
            packet = at - input.data + 4;
        }

        if (message == 0 && unique(at, 14)) {
            message = at - input.data + 14;
        }
        
        if (packet && message) { break; }
    }

    solve("{u}", packet);
    solve("{u}", message);
}
