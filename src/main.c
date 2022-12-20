#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "lib/common.h"
#include "lib/memory.h"

AllocCtx local_alloc;
double perf_freq;

static Solver *loadSolver(char *dir) {
    char path[12] = "XXXX/XX.dll";
    for (int i = 0; i < 7; ++i) { path[i] = dir[i]; }

    HMODULE dll = LoadLibraryA(path);
    if (!dll) { return NULL; }

    Solver *solver = (Solver *)GetProcAddress(dll, "_solver");
    return solver;
}

void runSolver(Solver *solver, char *dir) {
    if (dir) {
        print("{} DAY {}", (String){ (u8 *)dir, 4 }, (String){ (u8 *)dir + 5, 2 });
    }

    AllocCtx solver_alloc = {0};
    memSetCtx(&solver_alloc);

    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    Solution solution = solver();
    QueryPerformanceCounter(&t1);

    u64 time   = (u64)(1e6 * (double)(t1.QuadPart - t0.QuadPart) / perf_freq + 0.5);
    u64 memory = solver_alloc.used;

    print("    Part 1: {}", solution.one);
    print("    Part 2: {}", solution.two);
    print("      Time: {u}.{u,3,*0}ms", time / 1000, time % 1000);
    print("    Memory: {u}.{u,3,*0}MB", memory / 1048576,
        (u64)(memory * 1000.0 / 1048576.0 + 0.5) % 1000);

    memFreeAll();
    memSetCtx(&local_alloc);
}

int main(int argc, char **argv) {
    memSetCtx(&local_alloc);

    LARGE_INTEGER li_perf_freq;
    QueryPerformanceFrequency(&li_perf_freq);
    perf_freq = (double)li_perf_freq.QuadPart;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (i > 1) { print(""); }
            Solver *solver = loadSolver(argv[i]);
            if (solver) {
                runSolver(solver, argv[i]);
            } else {
                print("Could not link to {s}", argv[i]);
            }
        }
    } else {
        char dir[7] = "XXXX/XX";
        for (int year = 2015; year <= 2022; ++year) {
            dir[0] = '0' + (year / 1000) % 10;
            dir[1] = '0' + (year / 100 ) % 10;
            dir[2] = '0' + (year / 10  ) % 10;
            dir[3] = '0' + (year       ) % 10;
            for (int day = 1; day <= 25; ++day) {
                dir[5] = '0' + (day / 10) % 10;
                dir[6] = '0' + (day     ) % 10;

                Solver *solver = loadSolver(dir);
                if (solver) {
                    print("");
                    runSolver(solver, dir);
                }
            }
        }
    }
}
