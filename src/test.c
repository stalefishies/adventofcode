#include "aoc.h"

#define ARR_T u64
#define ARR_CMP
#include "arr.h"

#define SET_KEY u64
#include "set.h"

int main(void) {
    AllocCtx alloc = {};
    memSetCtx(&alloc);
    
    set_t set = {};
    for (us i = 0; i < 100; ++i) { setAdd(&set, i); }
    for (us i = 0; i < 100; i += 2) { setDelete(set, i); }

    u64 *arr = NULL;
    SetFor(set, i) { print("{u}", i); arrPush(&arr, i); }
    print("");

    arrSortInc(arr);
    ArrFor(arr, i) { print("{u}", i); }
}
