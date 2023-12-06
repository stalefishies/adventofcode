#include "aoc.h"

#define pPeekAccept(lit) pPeekAcceptCtx(&_p_ctx, strLiteral(lit))
bool pPeekAcceptCtx(String *ctx, const String key) {
    if (key.length > ctx->length) { return false; }

    for (us i = 0; i < key.length; ++i) {
        if (ctx->data[i] != key.data[i]) { return false; }
    }

    return true;
}

void solver(void) {
    s64 sum = 0;

    PARSE_INPUT("\n") {
        u8 first = 0;
        u8 last  = 0;

        while (!pDone()) {
            u8 value = 0;

            if (pPeek() >= '0' && pPeek() <= '9') {
                value = pPeek() - '0';
            } else if (pPeekAccept("one")) {
                value = 1;
            } else if (pPeekAccept("two")) {
                value = 2;
            } else if (pPeekAccept("three")) {
                value = 3;
            } else if (pPeekAccept("four")) {
                value = 4;
            } else if (pPeekAccept("five")) {
                value = 5;
            } else if (pPeekAccept("six")) {
                value = 6;
            } else if (pPeekAccept("seven")) {
                value = 7;
            } else if (pPeekAccept("eight")) {
                value = 8;
            } else if (pPeekAccept("nine")) {
                value = 9;
            }

            if (value > 0) {
                if (first == 0) { first = value; }
                last = value;
            }

            pChar();
        }

        sum += 10 * first + last;
    }

    solve("{d}", sum);
}
