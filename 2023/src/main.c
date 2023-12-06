#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "common.h"
#include "memory.h"

AllocCtx local_alloc;
double perf_freq;

bool run(char day[static 2], bool expected) {
    char DLL_0[7] = "XX.dll";
    char DLL_1[9] = "XX-1.dll";
    char DLL_2[9] = "XX-2.dll";
    DLL_0[0] = DLL_1[0] = DLL_2[0] = day[0];
    DLL_0[1] = DLL_1[1] = DLL_2[1] = day[1];

    Solution solution;
    double perf_time;
    AllocCtx solver_alloc = {0};
    memSetCtx(&solver_alloc);

    HMODULE dll = LoadLibraryA(DLL_0);
    if (dll) {
        Solver *solver = (Solver *)GetProcAddress(dll, "_solver");
        if (!solver) {
            print("Could not load solver from {s}", DLL_0);
            return false;
        }

        print("\n2023 DAY {c}{c}", day[0], day[1]);

        LARGE_INTEGER t0, t1;
        QueryPerformanceCounter(&t0);
        solution = solver();
        QueryPerformanceCounter(&t1);
        perf_time = (double)(t1.QuadPart - t0.QuadPart);
    } else {
        dll = LoadLibraryA(DLL_1);
        if (!dll) {
            if (expected) { print("Could not link to day {c}{c}", day[0], day[1]); }
            return !expected;
        }

        Solver *solver = (Solver *)GetProcAddress(dll, "_solver");
        if (!solver) {
            print("Could not load solver from {s}", DLL_1);
            return false;
        }

        print("\n2023 DAY {c}{c}", day[0], day[1]);

        LARGE_INTEGER t0, t1;
        QueryPerformanceCounter(&t0);
        solution = solver();
        QueryPerformanceCounter(&t1);
        perf_time = (double)(t1.QuadPart - t0.QuadPart);

        dll = LoadLibraryA(DLL_2);
        if (dll) {
            solver = (Solver *)GetProcAddress(dll, "_solver");
            if (!solver) {
                print("Could not load solver from {s}", DLL_2);
                return false;
            }

            QueryPerformanceCounter(&t0);
            solution.two = solver().one;
            QueryPerformanceCounter(&t1);
            perf_time += (double)(t1.QuadPart - t0.QuadPart);
        }
    }

    u64 time   = (u64)(1e6 * perf_time / perf_freq + 0.5);
    u64 memory = solver_alloc.used;

    print("    Part 1: {}", solution.one);
    print("    Part 2: {}", solution.two);
    print("      Time: {u}.{u,3,*0}ms", time / 1000, time % 1000);
    print("    Memory: {u}.{u,3,*0}MB", memory / 1048576,
        (u64)(memory * 1000.0 / 1048576.0 + 0.5) % 1000);

    memFreeAll();
    memSetCtx(&local_alloc);
    return true;
}

int main(int argc, char **argv) {
    memSetCtx(&local_alloc);

    LARGE_INTEGER li_perf_freq;
    QueryPerformanceFrequency(&li_perf_freq);
    perf_freq = (double)li_perf_freq.QuadPart;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            char day[2];
            if (argv[i][0] && !argv[i][1]) {
                day[0] = '0';
                day[1] = argv[i][0];
            } else if (argv[i][0] && argv[i][1] && !argv[i][2]) {
                day[0] = argv[i][0];
                day[1] = argv[i][1];
            } else {
                print("Invalid day {s}", argv[i]);
                continue;
            }

            run(day, true);
        }
    } else {
        char day[3] = "XX";

        for (int d = 1; d <= 25; ++d) {
            day[0] = '0' + (d / 10) % 10;
            day[1] = '0' + (d     ) % 10;

            if (!run(day, false)) { break; }
        }
    }
}
