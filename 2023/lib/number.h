#ifndef NUMBER_HGUARD
#define NUMBER_HGUARD

#include "common.h"

// For given a, b, return gcd(a, b)
// Optionally, calculate x and y such that ax + by = gcd(a, b)
u64 numEuclid(u64 a, u64 b, u64 *x, u64 *y);

u64 numGCD(u64 a, u64 b);
u64 numLCM(u64 a, u64 b);

#endif /* NUMBER_HGUARD */


#ifdef  AOC_IMPLEMENTATION
#ifndef NUMBER_IMPLEMENTED
#define NUMBER_IMPLEMENTED

u64 numEuclid(u64 a, u64 b, u64 *px, u64 *py) {
    u64 prev_r = a, r = b;
    u64 prev_x = 1, x = 0;

    while (r > 0) {
        u64 quotient = prev_r / r;

        u64 this_r = prev_r - quotient * r;
        prev_r = r;
        r = this_r;

        u64 this_x = prev_x - quotient * x;
        prev_x = x;
        x = this_x;
    }

    if (px) { *px = prev_x; }
    if (py) { *py = (b > 0 ? (prev_r - prev_x * a) / b : 0); }

    return prev_x;
}

u64 numGCD(u64 a, u64 b) { return numEuclid(a, b, NULL, NULL); }
u64 numLCM(u64 a, u64 b) { return (a * b) / numGCD(a, b); }

#endif /* NUMBER_IMPLEMENTED */
#endif /* AOC_IMPLEMENTATION */
