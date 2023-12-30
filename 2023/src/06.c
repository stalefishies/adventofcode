#include "aoc.h"

#define ARR_T s64
#include "arr.h"

s64 countWins(s64 count, s64 *time, s64 *dist) {
    s64 product = 1;

    for (s64 i = 0; i < count; ++i) {
        s64 T = time[i];
        s64 d = dist[i];

        // binary search for smallest solution t to t * (T - t) > d
        s64 t = T / 4;
        for (s64 lo = 1, hi = T / 2; lo < hi;) {
            if (t * (T - t) > d) { hi = t; } else { lo = t + 1; }
            t = (lo + hi) / 2;
        }

        s64 ways = T - 2 * t + 1;
        product *= ways;
    }

    return product;
}

void solver(void) {
    s64 *times = NULL;
    s64 *dists = NULL;
    s64 time = 0;
    s64 dist = 0;

    PARSE_STRING(input) {
        pAssert("Time:");
        while (!pAccept("\n")) {
            while (pAccept(" "));

            s64 value = pNum();
            arrPush(&times, value);

            if      (value < 10)   { time *= 10;    }
            else if (value < 100)  { time *= 100;   }
            else if (value < 1000) { time *= 1000;  }
            else                   { time *= 10000; }
            time += value;
        }

        pAssert("Distance:");
        while (!pAccept("\n")) {
            while (pAccept(" "));

            s64 value = pNum();
            arrPush(&dists, value);

            if      (value < 10)   { dist *= 10;    }
            else if (value < 100)  { dist *= 100;   }
            else if (value < 1000) { dist *= 1000;  }
            else                   { dist *= 10000; }
            dist += value;
        }
    }

    solve("{d}", countWins(arrLen(times), times, dists));
    solve("{d}", countWins(1, &time, &dist));
}
