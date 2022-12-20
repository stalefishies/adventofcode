/********
 * ARR.H
 */

#ifndef ARR_HGUARD
#define ARR_HGUARD

#include <stddef.h>
#include "lib/memory.h"

#define ArrFor(arr, var)                                            \
    for (size_t _af_i_##__LINE__ = 0, _af_b_##__LINE__ = 1;         \
        _af_i_##__LINE__ < ((arr) ? *((size_t *)(arr) - 1) : 0);    \
        ++_af_i_##__LINE__, _af_b_##__LINE__ = 1)                   \
    for (__typeof__(*arr) var = (arr)[_af_i_##__LINE__];            \
        _af_b_##__LINE__;                                           \
        _af_b_##__LINE__ = 0)


#endif /* ARR_HGUARD */


#define ARR_EXPAND(token, a, b) token(a, b)
#define ARR_CONCAT(a, b) a##b

#ifndef ARR_T
#   define ARR_T s64
#endif

#ifndef ARR_N
#   define ARR_N arr
#endif

#ifndef ARR_PREFIX
#   define ARR_PREFIX(token) ARR_EXPAND(ARR_CONCAT, ARR_N, token)
#endif

#ifndef ARR_LINKAGE
#   define ARR_LINKAGE __attribute__((unused)) static
#endif

#ifndef ARR_ALLOC
#   define ARR_ALLOC(size) alloc(ARR_T, size)
#endif

#ifndef ARR_FREE
#   define ARR_FREE(ptr, size)
#endif

#if !defined(ARR_DECLARE) && !defined(ARR_DEFINE)
#   define ARR_DECLARE
#   define ARR_DEFINE
#endif

#if defined(ARR_EQ)
#   if (1-ARR_EQ-1 == 2) // Test for blank ARR_EQ
#       undef ARR_EQ
#       define ARR_EQ(a, b) ((a) == (b))
#   endif
#elif defined(ARR_EQ_AS)
#   define ARR_EQ(a, b) ((ARR_EQ_AS(a)) == (ARR_EQ_AS(b)))
#endif

#if defined(ARR_CMP)
#   if (1-ARR_CMP-1 == 2) // Test for blank ARR_CMP
#       undef ARR_CMP
#       define ARR_CMP(a, b) (((a) > (b)) - ((a) < (b)))
#   endif
#elif defined(ARR_CMP_AS)
#   define ARR_CMP(a, b) \
    (((ARR_CMP_AS(a)) > (ARR_CMP_AS(b))) - ((ARR_CMP_AS(a)) < (ARR_CMP_AS(b))))
#endif


/********
 * DECLARATIONS
 */

#ifdef ARR_DECLARE

ARR_LINKAGE size_t ARR_PREFIX(Len) (ARR_T *arr);
ARR_LINKAGE void   ARR_PREFIX(Free)(ARR_T *arr);
ARR_LINKAGE void   ARR_PREFIX(Push)(ARR_T **arr, ARR_T value);
ARR_LINKAGE ARR_T  ARR_PREFIX(Pop) (ARR_T **arr);

#ifdef ARR_EQ
ARR_LINKAGE bool ARR_PREFIX(Has)   (ARR_T *arr, ARR_T value);
ARR_LINKAGE s64  ARR_PREFIX(Search)(ARR_T *arr, ARR_T value);
#endif

#ifdef ARR_CMP
ARR_LINKAGE void ARR_PREFIX(SortInc)(ARR_T *arr);
ARR_LINKAGE void ARR_PREFIX(SortDec)(ARR_T *arr);
#endif

#endif /* ARR_DECLARE */


/********
 * DEFINITIONS
 */

#ifdef ARR_DEFINE

ARR_LINKAGE size_t ARR_PREFIX(Len)(ARR_T *arr) {
    return (arr ? *((size_t *)arr - 1) : 0);
}

ARR_LINKAGE void ARR_PREFIX(Free)(ARR_T *arr) {
    if (!arr) { return; }
    UNUSED size_t *base = (size_t *)arr - 2;
    UNUSED size_t  size = *base * sizeof(ARR_T) + 2 * sizeof(size_t);
    ARR_FREE(base, size);
}

ARR_LINKAGE void ARR_PREFIX(Push)(ARR_T **arr, ARR_T value) {
    size_t len = (*arr ? *((size_t *)(*arr) - 1) : 0);
    size_t cap = (*arr ? *((size_t *)(*arr) - 2) : 0);

    if (cap < 64 || len == cap) {
        size_t new_cap = cap < 64 ? 64 : 2 * cap;
        ARR_T *new_arr = ARR_ALLOC(new_cap * sizeof(ARR_T) + 2 * sizeof(size_t));

        new_arr = (ARR_T *)((size_t *)new_arr + 2);
        *((size_t *)new_arr - 1) = len;
        *((size_t *)new_arr - 2) = new_cap;

        for (size_t i = 0; i < len; ++i) { new_arr[i] = (*arr)[i]; }

        if (*arr) { ARR_FREE(*arr, capacity * sizeof(ARR_T) + 2 * sizeof(size_t)); }
        *arr = new_arr;
    }

    (*arr)[len] = value;
    (*((size_t *)(*arr) - 1))++;
}

ARR_LINKAGE ARR_T ARR_PREFIX(Pop)(ARR_T **arr) {
    size_t len = (*arr ? *((size_t *)(*arr) - 1) : 0);
    if (len == 0) { return (ARR_T){0}; }

    (*((size_t *)(*arr) - 1))--;
    return (*arr)[len - 1];
}

#ifdef ARR_EQ

ARR_LINKAGE s64 ARR_PREFIX(Has)(ARR_T *arr, ARR_T value) {
    size_t len = (*arr ? *((size_t *)(*arr) - 1) : 0);
    for (s64 i = 0; i < len; ++i) {
        if (ARR_EQ(arr[i], key)) { return true; }
    }

    return false;
}

ARR_LINKAGE s64 ARR_PREFIX(Search)(ARR_T *arr, ARR_T value) {
    s64 index = -1;

    size_t len = (*arr ? *((size_t *)(*arr) - 1) : 0);
    for (s64 i = 0; i < len; ++i) {
        if (ARR_EQ(arr[i], key)) { index = i; break; }
    }

    return index;
}

#endif

#ifdef ARR_CMP

#define ARR_SWAP(arr, i, j) do {    \
    ARR_T tmp = arr[(i)];           \
    arr[(i)]  = arr[(j)];           \
    arr[(j)]  = tmp;                \
} while (0)

static void ARR_PREFIX(SortInsert)(ARR_T *arr, size_t N, s64 sign) {
    for (us i = 1; i < N; ++i) {
        for (us j = i; j > 0; --j) {
            if (sign * (s64)(ARR_CMP(arr[j], arr[j - 1])) < 0) {
                ARR_SWAP(arr, j, j - 1);
            } else { break; }
        }
    }
}

static void ARR_PREFIX(SortQuick)(ARR_T *arr, size_t N, s64 sign) {
    if (N <= 16) { ARR_PREFIX(SortInsert)(arr, N, sign); return; }

    // median-of-three pivot selection: move pivot to index 0
    if ((ARR_CMP(arr[N / 2], arr[0])) != (ARR_CMP(arr[N / 2], arr[N - 1]))) {
        ARR_SWAP(arr, 0, N / 2);
    } else if ((ARR_CMP(arr[N - 1], arr[0])) != (ARR_CMP(arr[N - 1], arr[N / 2]))) {
        ARR_SWAP(arr, 0, N - 1);
    }

    us i = 0, j = 1, k = N;
    while (j < k) {
        s64 cmp = sign * (ARR_CMP(arr[i], arr[j]));
        if (cmp > 0) {
            ARR_SWAP(arr, i, j);
            ++i; ++j;
        } else if (cmp < 0) {
            --k;
            ARR_SWAP(arr, j, k);
        } else { ++j; }
    }

    ARR_PREFIX(SortQuick)(arr,     i,     sign);
    ARR_PREFIX(SortQuick)(arr + k, N - k, sign);
}

ARR_LINKAGE void ARR_PREFIX(SortInc)(ARR_T *arr) {
    if (!arr) { return; }
    ARR_PREFIX(SortQuick)(arr, *((size_t *)arr - 1), 1);
}

ARR_LINKAGE void ARR_PREFIX(SortDec)(ARR_T *arr) {
    if (!arr) { return; }
    ARR_PREFIX(SortQuick)(arr, *((size_t *)arr - 1), -1);
}

#undef ARR_SWAP
#endif

#endif /* ARR_DEFINE */


/********
 * CLEANUP
 */

#undef ARR_EXPAND
#undef ARR_CONCAT

#ifdef ARR_T
#   undef ARR_T
#endif
#ifdef ARR_N
#   undef ARR_N
#endif
#ifdef ARR_PREFIX
#   undef ARR_PREFIX
#endif
#ifdef ARR_LINKAGE
#   undef ARR_LINKAGE
#endif
#ifdef ARR_ALLOC
#   undef ARR_ALLOC
#endif
#ifdef ARR_FREE
#   undef ARR_FREE
#endif
#ifdef ARR_DECLARE
#   undef ARR_DECLARE
#endif
#ifdef ARR_DEFINE
#   undef ARR_DEFINE
#endif
#ifdef ARR_CMP
#   undef ARR_CMP
#endif
#ifdef ARR_CMP_AS
#   undef ARR_CMP_AS
#endif
