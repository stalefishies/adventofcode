#include "aoc.h"

#define STAGE_COUNT 7

typedef struct Map {
    s64 src, dst, count;
} Map;
#define ARR_T Map
#define ARR_N map
#include "arr.h"

void solver(void) {
    Map *seeds[2] = {0};
    Map *stage[STAGE_COUNT] = {0};

    PARSE_STRING(input) {
        pAssert("seeds:");

        while (!pAccept("\n")) {
            pAssert(" ");
            s64 seed_1 = pNum();
            pAssert(" ");
            s64 seed_2 = pNum();

            mapPush(&seeds[0], (Map){ seed_1, seed_1, 1 });
            mapPush(&seeds[0], (Map){ seed_2, seed_2, 1 });
            mapPush(&seeds[1], (Map){ seed_1, seed_1, seed_2 });
        }

        for (s64 i = 0; i < STAGE_COUNT; ++i) {
            pAssert("\n");
            pSplit("\n");
            while (!pDone() && pPeek() != '\n') {
                Map map;
                map.dst = pNum();
                pAssert(" ");
                map.src = pNum();
                pAssert(" ");
                map.count = pNum();
                pAssert("\n");
                mapPush(&stage[i], map);
            }
        }
    }

    for (us k = 0; k < 2; ++k) {
        for (us n = 0; n < STAGE_COUNT; ++n) {
            Map *maps = stage[n];
            for (us i = 0; i < mapLen(seeds[k]); ++i) {
                for (us j = 0; j < mapLen(maps); ++j) {
                    Map *seed = &seeds[k][i];
                    Map  map  = maps[j];

                    if (seed->dst < map.src) {
                        if (seed->dst + seed->count <= map.src) { continue; }

                        s64 split = map.src - seed->dst;
                        Map new_map = { seed->src, seed->dst, split };
                        mapPush(&seeds[k], new_map);

                        seed->src   += split;
                        seed->dst   += split;
                        seed->count -= split;
                    }

                    if (seed->dst + seed->count > map.src + map.count) {
                        if (seed->dst >= map.src + map.count) { continue; }

                        s64 split = (seed->dst + seed->count) - (map.src + map.count);
                        Map new_map = { seed->src + seed->count - split,
                            seed->dst + seed->count - split, split };
                        mapPush(&seeds[k], new_map);

                        seed->count -= split;
                    }

                    seed->dst += map.dst - map.src;
                    break;
                }
            }
        }

        s64 min_seed = seeds[k][0].dst;
        for (us i = 1; i < mapLen(seeds[k]); ++i) {
            min_seed = seeds[k][i].dst < min_seed ? seeds[k][i].dst : min_seed;
        }

        solve("{d}", min_seed);
    }
}
