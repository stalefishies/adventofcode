#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "common.h"
#include "memory.h"

AllocCtx local_alloc;
double perf_freq;

static Solver *loadSolver(char *day) {
    char path[7] = "XX.dll";
    path[0] = day[0];
    path[1] = day[1];

    HMODULE dll = LoadLibraryA(path);
    if (!dll) { return NULL; }

    Solver *solver = (Solver *)GetProcAddress(dll, "_solver");
    return solver;
}

void runSolver(Solver *solver, char *day) {
    if (day) {
        print("2022 DAY {s}", day);
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
        char day[3] = "XX";

        for (int d = 1; d <= 25; ++d) {
            day[0] = '0' + (d / 10) % 10;
            day[1] = '0' + (d     ) % 10;

            Solver *solver = loadSolver(day);
            if (solver) {
                print("");
                runSolver(solver, day);
            } else { break; }
        }
    }
}
