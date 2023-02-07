#include "aoc.h"

#define ARR_T s64
#include "arr.h"

s64 mix(s64 *file, s64 key, us count) {
    us length = arrLen(file);

    u64 *mix = NULL;
    for (us i = 0; i < length; ++i) {
        arrPush((s64 **)&mix, i);
    }

    for (us n = 0; n < count; ++n) {
        for (us i = 0; i < length; ++i) {
            s64 value = file[i] * key;

            us pos = 0;
            for (us j = 0; j < length; ++j) {
                if (mix[j] == i) {
                    pos = j;
                    break;
                }
            }

            u64 shift = (value > 0 ? value : -value) % (length - 1);
            s64 dir   = (value > 0 ? 1 : -1);

            for (us n = 0; n < shift; ++n) {
                us new_pos = (pos + dir + length) % length;
                mix[pos] = mix[new_pos];
                pos = new_pos;
            }
            mix[pos] = i;
        }
    }

    us zero_pos = 0;
    for (us i = 0; i < length; ++i) {
        if (file[mix[i]] == 0) {
            zero_pos = i;
            break;
        }
    }

    s64 sum = (file[mix[(zero_pos + 1000) % length]]
        + file[mix[(zero_pos + 2000) % length]]
        + file[mix[(zero_pos + 3000) % length]]) * key;
    return sum;
}

void solver(void) {
    s64 *file = NULL;
    PARSE_INPUT("\n") { arrPush(&file, pNum()); }

    solve("{d}", mix(file, 1, 1));
    solve("{d}", mix(file, 811589153, 10));
}
