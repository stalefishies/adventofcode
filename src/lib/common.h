#ifndef COMMON_HGUARD
#define COMMON_HGUARD

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#define EXPORT __declspec(dllexport)
#define UNUSED __attribute__((unused))

#if defined(YEAR) && !(YEAR + 0 >= 2015)
#   error Invalid year
#endif

#if defined(DAY) && !(DAY + 0 >= 1)
#   error Invalid day
#endif

#if defined(YEAR) && defined(DAY)
#   define AOC_SOLUTION 1
#endif

typedef   int8_t s8;
typedef  int16_t s16;
typedef  int32_t s32;
typedef  int64_t s64;
typedef  uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef   size_t us;

typedef struct { u8 *data; us length; } String;
#define strLiteral(literal) ((String){ .data = (u8 *)(literal), .length = (sizeof(literal) - 1) })
#define strArg(string) (int)(string).length, (string).data

typedef struct AllocCtx AllocCtx;
typedef struct Solution { String one, two; } Solution;
typedef Solution Solver(void);

EXPORT String print(const char *fmt, ...);
EXPORT void _Noreturn error(const char *fmt, ...);

#endif /* COMMON_HGUARD */
