#include "aoc.h"

typedef struct Mat {
    u8 or, cl, ob, ge;
} Mat;

typedef struct Blueprint {
    Mat or;
    Mat cl;
    Mat ob;
    Mat ge;
} Blueprint;

#define ARR_T Blueprint
#define ARR_N bp
#include "arr.h"

#define RECURSE(MAT) do {                                                   \
    Mat new_res = {                                                         \
        res.or + prod.or - cost.MAT.or,                                     \
        res.cl + prod.cl - cost.MAT.cl,                                     \
        res.ob + prod.ob - cost.MAT.ob,                                     \
        res.ge + prod.ge - cost.MAT.ge                                      \
    };                                                                      \
    Mat new_prod = { prod.or, prod.cl, prod.ob, prod.ge };                  \
    ++new_prod.MAT;                                                         \
    Mat new_can = { 1, 1, 1, 1 };                                           \
    u64 new = simulate(cost, time - 1, new_res, new_prod, new_can, max);    \
    if (new > best) { best = new; }                                         \
} while (0)

u64 simulate(Blueprint cost, u64 time, Mat res, Mat prod, Mat can, Mat max) {
    if (time == 0) { return res.ge; }
    u64 best = res.ge;

    if (res.or >= cost.ge.or && res.ob >= cost.ge.ob) {
        RECURSE(ge);
    } else {
        if (can.or && prod.or < max.or && res.or >= cost.or.or) {
            RECURSE(or);
        }

        if (can.cl && prod.cl < max.cl && res.or >= cost.cl.or) {
            RECURSE(cl);
        }

        if (can.ob && prod.ob < max.ob && res.or >= cost.ob.or && res.cl >= cost.ob.cl) {
            RECURSE(ob);
        }

        {
            Mat new_res = { res.or + prod.or, res.cl + prod.cl, res.ob + prod.ob, res.ge + prod.ge };
            Mat new_can = {
                can.or && res.or < cost.or.or,
                can.cl && res.or < cost.cl.or,
                can.ob && (res.or < cost.ob.or || res.cl < cost.ob.cl),
                1,
            };

            u64 new = simulate(cost, time - 1, new_res, prod, new_can, max);
            if (new > best) { best = new; }
        }
    }

    return best;
}

void solver(void) {
    Blueprint *blueprints = NULL;

    PARSE_INPUT("\n") {
        Blueprint cost = {0};

        pAssert("Blueprint ");
        pNum();
        pAssert(": Each ore robot costs ");
        cost.or.or = pNum();
        pAssert(" ore. Each clay robot costs ");
        cost.cl.or = pNum();
        pAssert(" ore. Each obsidian robot costs ");
        cost.ob.or = pNum();
        pAssert(" ore and ");
        cost.ob.cl = pNum();
        pAssert(" clay. Each geode robot costs ");
        cost.ge.or = pNum();
        pAssert(" ore and ");
        cost.ge.ob = pNum();

        bpPush(&blueprints, cost);
    }

    us quality = 0;
    us product = 1;

    for (us i = 0; i < bpLen(blueprints); ++i) {
        Blueprint bp = blueprints[i];

        Mat res  = { 0, 0, 0, 0 };
        Mat prod = { 1, 0, 0, 0 };
        Mat can  = { 1, 1, 1, 1 };

        Mat max_prod = { bp.or.or, bp.ob.cl, bp.ge.ob, 255 };
        if (bp.cl.or > max_prod.or) { max_prod.or = bp.cl.or; }
        if (bp.ob.or > max_prod.or) { max_prod.or = bp.ob.or; }
        if (bp.ge.or > max_prod.or) { max_prod.or = bp.ge.or; }

        u8 best = simulate(bp, 24, res, prod, can, max_prod);
        quality += best * (i + 1);

        if (i < 3) {
            u8 best_32 = simulate(bp, 32, res, prod, can, max_prod);
            product *= best_32;
        }
    }

    solve("{u}", quality);
    solve("{u}", product);
}
