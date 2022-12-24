#include "aoc.h"

#define SET_KEY  String
#define SET_MAP  u8
#define SET_EQ   strEq
#define SET_HASH strHash
#include "set.h"

typedef struct Valve {
    String name;
    u8 *dist;
    u8 full;
    u8 flow;
    u8 init;
    u8 used;
} Valve;

#define ARR_T Valve
#define ARR_CMP_AS(v) v.flow
#include "arr.h"

static us N;
static Valve *v;

void run(u64 time, u64 pressure, u64 *best,
    u8 dest_1, u8 dist_1, u8 dest_2, u8 dist_2)
{
    us max = pressure
        + v[dest_1].flow * (dist_1 < time ? time - dist_1 : 0)
        + v[dest_2].flow * (dist_2 < time ? time - dist_2 : 0);
    for (us i = 0; i < N; ++i) {
        if (v[i].used) { continue; }

        max += v[i].flow * time;
        continue;

        if (v[i].dist[dest_1] < v[i].dist[dest_2]) {
            max += v[i].flow * (time - v[i].dist[dest_1]);
        } else {
            max += v[i].flow * (time - v[i].dist[dest_2]);
        }
    }
    if (max <= *best) { return; }

    if (dist_1 == 0) {
        pressure += v[dest_1].flow * time;
        if (pressure > *best) { *best = pressure; }
    }

    if (dist_2 == 0) {
        pressure += v[dest_2].flow * time;
        if (pressure > *best) { *best = pressure; }
    }

    if (dist_1 == 0 && dist_2 == 0) {
        for (u8 next_1 = 0; next_1 < N; ++next_1) {
            if (v[next_1].used) { continue; }
            v[next_1].used = 1;

            for (u8 next_2 = 0; next_2 < N; ++next_2) {
                if (v[next_2].used) { continue; }
                v[next_2].used = 2;

                run(time, pressure, best,
                    next_1, v[dest_1].dist[next_1],
                    next_2, v[dest_2].dist[next_2]);

                v[next_2].used = 0;
            }

            v[next_1].used = 0;
        }

        return;
    } else if (dist_1 == 0) {
        bool done = true;

        for (u8 next = 0; next < N; ++next) {
            if (v[next].used) { continue; }
            v[next].used = 1;

            done = false;
            run(time, pressure, best,
                next, v[dest_1].dist[next],
                dest_2, dist_2);

            v[next].used = 0;
        }

        if (done && time > dist_2) {
            pressure += v[dest_2].flow * (time - dist_2);
            if (pressure > *best) { *best = pressure; }
        }
    } else if (dist_2 == 0) {
        bool done = true;

        for (u8 next = 0; next < N; ++next) {
            if (v[next].used) { continue; }
            v[next].used = 2;

            done = false;
            run(time, pressure, best,
                dest_1, dist_1,
                next, v[dest_2].dist[next]);

            v[next].used = 0;
        }

        if (done && time > dist_1) {
            pressure += v[dest_1].flow * (time - dist_1);
            if (pressure > *best) { *best = pressure; }
        }
    } else {
        u8 move = dist_1 < dist_2 ? dist_1 : dist_2;
        if (time < move) { return; }

        run(time - move, pressure, best,
            dest_1, dist_1 - move,
            dest_2, dist_2 - move);
    }
}

void solver(void) {
    set_t full_set = {};

    us full_N = 0;
    PARSE_INPUT("\n") {
        pAssert("Valve ");
        setAdd(&full_set, pString(2), full_N++);
    }

    String *full_name = alloc(String, full_N);
    u64 *full_flow = alloc(u64, full_N);
    u8  *full_dist = alloc(u8, full_N * full_N);

    for (us i = 0; i < full_N; ++i) {
        for (us j = 0; j < full_N; ++j) {
            full_dist[i * full_N + j] = (i == j ? 0x00 : 0xFF);
        }
    }

    us full_index = 0;
    PARSE_INPUT("\n") {
        pAssert("Valve ");
        full_name[full_index] = pString(2);
        pAssert(" has flow rate=");
        full_flow[full_index] = pNum();
        pAssert("; tunnel");

        if (pAccept(" ")) {
            pAssert("leads to valve ");
            u8 neighbour = setGet(full_set, pString(2));
            full_dist[full_index * full_N + neighbour] = 1;
        } else {
            pAssert("s lead to valves ");
            do {
                u8 neighbour = setGet(full_set, pString(2));
                full_dist[full_index * full_N + neighbour] = 1;
            } while (pAccept(", "));
        }

        ++full_index;
    }

    for (bool changed = true; changed;) {
        changed = false;

        for (us i = 0; i < full_N; ++i) {
            for (us j = 0; j < full_N; ++j) {
                for (us n = 0; n < full_N; ++n) {
                    if (full_dist[i * full_N + n] == 0xFF) { continue; }
                    if (full_dist[n * full_N + j] == 0xFF) { continue; }
                    if (full_dist[i * full_N + j] == 0xFF
                        || full_dist[i * full_N + j] >
                        full_dist[i * full_N + n] + full_dist[n * full_N + j])
                    {
                        full_dist[i * full_N + j] =
                            full_dist[i * full_N + n] + full_dist[n * full_N + j];
                        changed = true;
                    }
                }
            }
        }
    }

    u8 index_AA = setGet(full_set, strLiteral("AA"));
    for (us i = 0; i < full_N; ++i) {
        if (full_flow[i] == 0) { continue; }

        Valve valve = {};
        valve.name = full_name[i];
        valve.full = i;
        valve.flow = full_flow[i];
        valve.init = full_dist[index_AA * full_N + i] + 1;
        valve.used = 0;
        arrPush(&v, valve);
    }

    N = arrLen(v);
    arrSortDec(v);

    for (us i = 0; i < N; ++i) {
        v[i].dist = alloc(u8, N);
        for (us j = 0; j < N; ++j) {
            v[i].dist[j] = full_dist[v[i].full * full_N + v[j].full] + 1;
        }
    }

    u64 part_1 = 0;
    for (us start = 0; start < N; ++start) {
        v[start].used = 1;
        run(30, 0, &part_1, start, v[start].init, 0, 99);
        v[start].used = 0;
    }
    solve("{u}", part_1);

    u64 part_2 = 0;
    for (us start_1 = 0; start_1 < N; ++start_1) {
        v[start_1].used = 1;

        for (us start_2 = start_1 + 1; start_2 < N; ++start_2) {
            v[start_2].used = 2;
            run(26, 0, &part_2, start_1, v[start_1].init, start_2, v[start_2].init);
            v[start_2].used = 0;
        }

        v[start_1].used = 0;
    }
    solve("{u}", part_2);
}
