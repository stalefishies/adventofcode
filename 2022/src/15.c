#include "aoc.h"

#define N 26
typedef struct Sensor { s32 i, x, y, d; } Sensor;

#define abs(x) ((x) < 0 ? -(x) : (x))
#define SENSOR_L(i, Y) (sensors[i].x - sensors[i].d + abs(sensors[i].y - (Y)))
#define SENSOR_R(i, Y) (sensors[i].x + sensors[i].d - abs(sensors[i].y - (Y)))

us sortSensors(Sensor sensors[N], s32 Y) {
    us n = N;

    for (us i = 1; i < n;) {
        if (SENSOR_R(i, Y) < SENSOR_L(i, Y)) {
            --n;
            Sensor tmp = sensors[i];
            sensors[i] = sensors[n];
            sensors[n] = tmp;
            continue;
        }
        
        for (us j = i; j > 0; --j) {
            if (SENSOR_L(j, Y) < SENSOR_L(j - 1, Y)
                || (SENSOR_L(j, Y) == SENSOR_L(j - 1, Y)
                && SENSOR_R(j, Y) < SENSOR_R(j - 1, Y)))
            {
                Sensor tmp = sensors[j];
                sensors[j] = sensors[j - 1];
                sensors[j - 1] = tmp;
            } else { break; }
        }

        ++i;
    }

    return n;
}

void solver(void) {
    Sensor sensors[N];

    us sensor_index = 0;
    PARSE_INPUT("\n") {
        pAssert("Sensor at x=");
        s32 x = pNum();
        pAssert(", y=");
        s32 y = pNum();
        pAssert(": closest beacon is at x=");
        s32 b_x = pNum();
        pAssert(", y=");
        s32 b_y = pNum();
        s32 dist = abs(x - b_x) + abs(y - b_y);

        sensors[sensor_index].i = sensor_index;
        sensors[sensor_index].x = x;
        sensors[sensor_index].y = y;
        sensors[sensor_index].d = dist;
        ++sensor_index;
    }

    s32 row_Y = 2000000;
    s32 min_x = SENSOR_L(0, row_Y);
    s32 max_x = SENSOR_R(0, row_Y);

    for (us i = 0; i < N; ++i) {
        if (SENSOR_R(i, row_Y) < SENSOR_L(i, row_Y)) { continue; }
        if (SENSOR_L(i, row_Y) < min_x) { min_x = SENSOR_L(i, row_Y); }
        if (SENSOR_R(i, row_Y) > max_x) { max_x = SENSOR_R(i, row_Y); }
    }

    solve("{d32}", max_x - min_x);

    for (s32 Y = 0; Y < 4000000; ++Y) {
        us n = sortSensors(sensors, Y);

        s32 curr_r = 0;
        for (us i = 0; i < n; ++i) {
            s32 this_l = SENSOR_L(i, Y);
            s32 this_r = SENSOR_R(i, Y);

            if (this_l <       0) { this_l =       0; }
            if (this_l > 4000000) { this_l = 4000000; }
            if (this_r <       0) { this_r =       0; }
            if (this_r > 4000000) { this_r = 4000000; }

            if (this_l > curr_r) {
                s64 freq = (s64)4000000 * (curr_r + 1) + Y;
                solve("{d}", freq);
                return;
            }
            
            if (this_r > curr_r) { curr_r = this_r; }
        }
    }
}
