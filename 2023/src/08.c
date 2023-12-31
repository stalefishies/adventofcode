#include "aoc.h"

#define ARR_T u8
#define ARR_N dir
#include "arr.h"

typedef struct { u32 l, r; } Node;
#define SET_KEY u32
#define SET_MAP Node
#define SET_PREFIX node
#include "set.h"

typedef struct { u32 dest, dist; } Step;
#define SET_KEY u32
#define SET_MAP Step
#define SET_PREFIX step
#include "set.h"

#define LOC(str) (((u32)((str)[0]) << 16) | ((u32)((str)[1]) << 8) | ((u32)((str)[2])))

void solver(void) {
    u8 *dirs = NULL;
    node_t nodes = {0};

    PARSE_STRING(input) {
        while (!pAccept("\n\n")) { dirPush(&dirs, pChar()); }

        while (!pDone()) {
            Node node;
            u32 key = ((u32)pChar() << 16) | ((u32)pChar() << 8) | pChar();
            pAssert(" = (");
            node.l = ((u32)pChar() << 16) | ((u32)pChar() << 8) | pChar();
            pAssert(", ");
            node.r = ((u32)pChar() << 16) | ((u32)pChar() << 8) | pChar();
            pAssert(")\n");

            nodeAdd(&nodes, key, node);
        }
    }

    us dir_count = dirLen(dirs);
    struct step_t starts = {0};
    struct step_t ends   = {0};

    SetFor(nodes, loc) {
        struct step_t *set = NULL;
        if ((loc & 0xFF) == 'A') { set = &starts; }
        if ((loc & 0xFF) == 'Z') { set = &ends;   }
        if (!set) { continue; }

        u32 at = loc;
        u32 dist = 0;
        do {
            Node node = nodeGet(nodes, at);
            bool dir = dirs[dist % dir_count] == 'L';
            at = (dir ? node.l : node.r);
            ++dist;
        } while ((at & 0xFF) != 'Z');

        Step step = { .dest = at, .dist = dist };
        stepAdd(set, loc, step);
    }

    {
        Step step = stepGet(starts, LOC("AAA"));
        u64 dist = step.dist;

        while (step.dest != LOC("ZZZ")) {
            step = stepGet(ends, step.dest);
            dist += step.dist;
        }

        solve("{d}", dist);
    }

    // Part 2, assuming:
    //  Each XXA leads to a different XXZ
    //  Each XXZ only leads back to itself
    //  The distance from XXA to XXZ is the same as XXZ to itself

    s64 total_dist = 1;
    SetFor(ends, loc) {
        Step step = stepGet(ends, loc);
        total_dist = numLCM(total_dist, step.dist);
    }
    solve("{u}", total_dist);
}
