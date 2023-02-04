#include "lib/common.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "lib/inputs.h"
#include "lib/parser.h"
#include "lib/number.h"

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

    input = readInputFile(YEAR, DAY);
    solver();

    return _solution;
}

#endif
