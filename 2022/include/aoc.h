#include "common.h"
#include "memory.h"
#include "string.h"
#include "inputs.h"
#include "parser.h"
#include "number.h"

#if defined(AOC_SOLUTION) || defined(__clang_analyzer__)

EXPORT Solution _solution;
EXPORT String input;

UNUSED static void solve(const char *fmt, ...) {
    String *string;

    if (_solution.one.data == NULL) {
        string = &_solution.one;
    } else if (_solution.two.data == NULL) {
        string = &_solution.two;
    } else {
        error("solve() called more than twice");
    }

    va_list args;
    va_start(args, fmt);
    *string = vformat(fmt, args);
    va_end(args);
}

EXPORT inline Solution _solver(void) {
    void solver(void);

    input = readInputFile(DAY);
    solver();

    return _solution;
}

#endif
