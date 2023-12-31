#ifndef NUMBER_HGUARD
#define NUMBER_HGUARD

#include "common.h"

// For given a, b, return gcd(a, b)
// Optionally, calculate x and y such that ax + by = gcd(a, b)
EXPORT s64 numEuclid(s64 a, s64 b, s64 *x, s64 *y);

EXPORT s64 numGCD(s64 a, s64 b);
EXPORT s64 numLCM(s64 a, s64 b);

#endif /* NUMBER_HGUARD */


#ifdef  AOC_IMPLEMENTATION
#ifndef NUMBER_IMPLEMENTED
#define NUMBER_IMPLEMENTED

s64 numEuclid(s64 a, s64 b, s64 *px, s64 *py) {
    s64 prev_r = a, r = b;
    s64 prev_x = 1, x = 0;

    while (r != 0) {
        s64 quotient = prev_r / r;

        s64 this_r = prev_r - quotient * r;
        prev_r = r;
        r = this_r;

        s64 this_x = prev_x - quotient * x;
        prev_x = x;
        x = this_x;
    }

    if (px) { *px = prev_x; }
    if (py) { *py = (b != 0 ? (prev_r - prev_x * a) / b : 0); }

    return prev_r;
}

s64 numGCD(s64 a, s64 b) { return numEuclid(a, b, NULL, NULL); }
s64 numLCM(s64 a, s64 b) { return (a * b) / numGCD(a, b); }

#endif /* NUMBER_IMPLEMENTED */
#endif /* AOC_IMPLEMENTATION */
