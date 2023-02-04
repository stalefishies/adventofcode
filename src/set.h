/********
 * SET.H
 *
 * Two modes of storage: indirect (default) or direct
 *
 * Indirect: we take in the data by value and copy them to our own storage. Each hash slot is a pointer to the item in our storage. NULL represents an empty slot.
 *
 * Direct: we take in the data by value. Each hash slot is the value itself. The caller can supply an invalid value representing an empty slot, which by default is zero.
 *
 * Direct can be used to store by reference, as a hash set of pointers.
 */

#ifndef SET_HGUARD
#define SET_HGUARD

#include <stdint.h>
#include "lib/memory.h"

#define SET_EXPAND(token, a, b, c) token(a, b, c)
#define SET_CONCAT(a, b, c) a##b##c

#define SetFor(set, var)                                \
    for (size_t                                         \
            _sf_i_##__LINE__ = 0,                       \
            _sf_b_##__LINE__ = 1;                       \
        _sf_i_##__LINE__ < (set).cap;                   \
        ++_sf_i_##__LINE__,                             \
            _sf_b_##__LINE__ = 1)                       \
    if ((set).slot[_sf_i_##__LINE__] != NULL)           \
    for (__typeof__((**(set).slot).key)                 \
            var = (set).slot[_sf_i_##__LINE__]->key;    \
        _sf_b_##__LINE__;                               \
        _sf_b_##__LINE__ = 0)

#endif /* SET_HGUARD */


/********
 * MACRO OPTIONS
 */

#ifndef SET_KEY
#   define SET_KEY s64
#endif

#ifndef SET_PREFIX
#   define SET_PREFIX set
#endif

#ifndef SET_LINKAGE
#   define SET_LINKAGE __attribute__((unused)) static
#endif

#ifndef SET_ALLOC
#   define SET_ALLOC(size) memAlloc(size)
#endif

#ifndef SET_FREE
#   define SET_FREE(ptr, size)
#endif

#if !defined(SET_DECLARE) && !defined(SET_TYPEDEF) && !defined(SET_DEFINE)
#   define SET_DECLARE
#   define SET_TYPEDEF
#   define SET_DEFINE
#endif

#ifndef SET_EQ
#   ifdef SET_EQ_AS
#       define SET_EQ(a, b) (SET_EQ_AS(a)) == (SET_EQ_AS(b))
#   else
#       define SET_EQ(a, b) (a) == (b)
#   endif
#endif

#ifndef SET_HASH
#   define SET_HASH(x) x
#endif

#ifdef SET_DIRECT
#   if (1-SET_DIRECT-1 == 2) // Test for blank SET_DIRECT
#       define SET_NULL 0
#   else
#       define SET_NULL SET_DIRECT
#   endif
#   error direct set storage not written yet
#else
#   define SET_NULL NULL
#endif

#ifndef SET_N
#   define SET_N(token) SET_EXPAND(SET_CONCAT, SET_PREFIX, token,)
#endif

#ifndef SET_T
#   define SET_T(token) SET_EXPAND(SET_CONCAT, SET_PREFIX, token, _t)
#endif


/********
 * DECLARATIONS
 */

#ifdef SET_DECLARE

typedef struct SET_T() SET_T();
SET_LINKAGE bool SET_N(SlotIndex)(SET_T() set, size_t index);

SET_LINKAGE bool     SET_N(Mem)(SET_T() *set, size_t  cap);
SET_LINKAGE bool     SET_N(Has)(SET_T()  set, SET_KEY key);
SET_LINKAGE bool     SET_N(Del)(SET_T()  set, SET_KEY key);
#ifdef SET_MAP
SET_LINKAGE SET_MAP *SET_N(Ptr)(SET_T()  set, SET_KEY key);
SET_LINKAGE SET_MAP  SET_N(Get)(SET_T()  set, SET_KEY key);
SET_LINKAGE bool     SET_N(Add)(SET_T() *set, SET_KEY key, SET_MAP map);
SET_LINKAGE bool     SET_N(Set)(SET_T() *set, SET_KEY key, SET_MAP map);
#else
SET_LINKAGE SET_KEY *SET_N(Ptr)(SET_T()  set, SET_KEY key);
SET_LINKAGE SET_KEY  SET_N(Get)(SET_T()  set, SET_KEY key);
SET_LINKAGE bool     SET_N(Add)(SET_T() *set, SET_KEY key);
SET_LINKAGE bool     SET_N(Set)(SET_T() *set, SET_KEY key);
#endif

#endif


/********
 * TYPE DEFINITIONS
 */

#ifdef SET_TYPEDEF

typedef SET_KEY SET_T(key);
#ifdef SET_MAP
typedef SET_MAP SET_T(map);
#endif

typedef union SET_T(pool) {
    struct {
        SET_T(key) key;
#ifdef SET_MAP
        SET_T(map) map;
#endif
    };
    union SET_T(pool) *free;
} SET_T(pool);

typedef union SET_T(pool) *SET_T(slot);

struct SET_T() {
    size_t cap;
    size_t num;
    SET_T(slot) *slot;
    SET_T(pool) *pool;
    SET_T(pool) *free;
};

#endif


/********
 * DEFINITIONS
 */

#ifdef SET_DEFINE

// 64-bit Fibonacci hash multiplicand
#define SET_FIB 11400714819323198485LLU

SET_LINKAGE bool SET_N(SlotIndex)(SET_T() set, size_t index) {
    return set.slot[index] != SET_NULL;
}

SET_LINKAGE bool SET_N(Mem)(SET_T() *set, size_t min_cap) {
    if (!set) { return false; }
    SET_T() new = {0};

    new.cap = (set->cap < 256 ? 256 : set->cap);
    if (new.cap < min_cap) { new.cap = min_cap; }

    // round up to next power of 2
    if (new.cap & (new.cap - 1)) {
        --new.cap;
        new.cap |= new.cap >>  1;
        new.cap |= new.cap >>  2;
        new.cap |= new.cap >>  4;
        new.cap |= new.cap >>  8;
        new.cap |= new.cap >> 16;
        new.cap |= new.cap >> 32;
        ++new.cap;
    }

    if (new.cap <= set->cap) { return false; }
    size_t dcap = new.cap / 2;
    u64    mask = new.cap - 1;

    new.slot = memAlloc(sizeof(SET_T(slot)) * new.cap + sizeof(SET_T(pool)) * dcap);
    for (us i = 0; i < new.cap; ++i) { new.slot[i] = SET_NULL; }

    new.pool = (SET_T(pool) *)(new.slot + new.cap);
    new.free = &new.pool[0];
    for (us i = 0; i < dcap - 1; ++i) { new.pool[i].free = &new.pool[i + 1]; }
    new.pool[dcap - 1].free = NULL;

    for (us i = 0; i < set->cap; ++i) {
        if (set->slot[i] == SET_NULL) { continue; }
        SET_T(pool) item = *set->slot[i];

        if (!new.free) { error("SET FATAL ERROR: No free slots"); }
        SET_T(pool) *pool = new.free;
        new.free = pool->free;
        *pool = item;

        u64 hash = (u64)(SET_HASH(item.key));
        size_t index = (SET_FIB * hash) & mask;
        while (new.slot[index] != SET_NULL) { index = (index + 1) & mask; }
        new.slot[index] = pool;
        ++new.num;
    }

    if (set->slot) {
        UNUSED size_t old_size = sizeof(SET_T(slot)) * set->cap
            + sizeof(SET_T(pool)) * set->cap / 2;
        SET_FREE(set->slot, old_size);
    }

    *set = new;
    return true;
}

static SET_T(slot) *SET_N(SlotPtr)(SET_T() set, SET_KEY key) {
    u64 hash = SET_HASH(key);
    u64 mask = set.cap - 1;
    size_t index = (SET_FIB * hash) & mask;

    while (set.slot[index] != SET_NULL) {
        if (SET_EQ(set.slot[index]->key, key)) { break; }
        index = (index + 1) & mask;
    }

    return &set.slot[index];
}

SET_LINKAGE bool SET_N(Del)(SET_T() set, SET_KEY key) {
    if (set.cap == 0) { return false; }

    u64 hash = SET_HASH(key);
    u64 mask = set.cap - 1;
    size_t index = (SET_FIB * hash) & mask;

    bool found = false;
    while (set.slot[index] != SET_NULL) {
        if (SET_EQ(set.slot[index]->key, key)) { found = true; break; }
        index = (index + 1) & mask;
    }
    if (!found) { return false; }

    set.slot[index]->free = set.free;
    set.free = set.slot[index];

    // swap index until deletion complete
    size_t del = index;
    for (;;) {
        size_t next = (index + 1) & mask;
        if (!set.slot[next]) { break; }

        size_t base = SET_FIB * (SET_HASH(set.slot[index + 1]->key)) & mask;
        if (next > del) {
            if (del < base && base <= next) { break; }
        } else {
            if (del < base || base <= next) { break; }
        }

        set.slot[index] = set.slot[next];
        index = next;
    }
    
    set.slot[index] = NULL;    
    return true;
}

SET_LINKAGE bool SET_N(Has)(SET_T() set, SET_KEY key) {
    if (set.cap == 0) { return false; }

    SET_T(slot) *slot = SET_N(SlotPtr)(set, key);
    return (slot && *slot);
}

#ifdef SET_MAP
SET_LINKAGE SET_MAP SET_N(Get)(SET_T() set, SET_KEY key)
#else
SET_LINKAGE SET_KEY SET_N(Get)(SET_T() set, SET_KEY key)
#endif
{
#ifdef SET_MAP
    if (set.cap == 0) { return (SET_MAP){0}; }
#else
    if (set.cap == 0) { return (SET_KEY){0}; }
#endif

    SET_T(slot) *slot = SET_N(SlotPtr)(set, key);
    if (slot && *slot) {
#ifdef SET_MAP
        return (*slot)->map;
    } else {
        return (SET_MAP){0};
#else
        return (*slot)->key;
    } else {
        return (SET_KEY){0};
#endif
    }
}

#ifdef SET_MAP
SET_LINKAGE SET_MAP *SET_N(Ptr)(SET_T() set, SET_KEY key)
#else
SET_LINKAGE SET_KEY *SET_N(Ptr)(SET_T() set, SET_KEY key)
#endif
{
    if (set.cap == 0) { return NULL; }

    SET_T(slot) *slot = SET_N(SlotPtr)(set, key);
    if (slot && *slot) {
#ifdef SET_MAP
        return &(*slot)->map;
#else
        return &(*slot)->key;
#endif
    } else {
        return NULL;
    }
}

#ifdef SET_MAP
SET_LINKAGE bool SET_N(Add)(SET_T() *set, SET_KEY key, SET_MAP map)
#else
SET_LINKAGE bool SET_N(Add)(SET_T() *set, SET_KEY key)
#endif
{
    u64 init_dcap = (set ? set->cap / 2 : 0);
    u64 init_num  = (set ? set->num     : 0);
    if (init_num >= init_dcap) { SET_N(Mem)(set, 4 * init_dcap); }

    SET_T(slot) *slot = SET_N(SlotPtr)(*set, key);
    if (*slot) { return false; }

    SET_T(pool) *data  = set->free;
    set->free = data->free;
    data->key = key;
#ifdef SET_MAP
    data->map  = map;
#endif
    *slot = data;
    ++set->num;

    return true;
}

#ifdef SET_MAP
SET_LINKAGE bool SET_N(Set)(SET_T() *set, SET_KEY key, SET_MAP map)
#else
SET_LINKAGE bool SET_N(Set)(SET_T() *set, SET_KEY key)
#endif
{
    u64 init_dcap = (set ? set->cap / 2 : 0);
    u64 init_num  = (set ? set->num     : 0);
    if (init_num >= init_dcap) { SET_N(Mem)(set, 4 * init_dcap); }

    SET_T(slot) *slot = SET_N(SlotPtr)(*set, key);
    if (*slot) {
        (*slot)->key = key;
#ifdef SET_MAP
        (*slot)->map = map;
#endif
        return true;
    } else {
        SET_T(pool) *data  = set->free;
        set->free = data->free;
        data->key = key;
#ifdef SET_MAP
        data->map  = map;
#endif
        *slot = data;
        ++set->num;
        return false;
    }
}

#endif



/********
 * CLEANUP
 */

#ifdef SET_KEY
#   undef SET_KEY
#endif
#ifdef SET_PREFIX
#   undef SET_PREFIX
#endif
#ifdef SET_LINKAGE
#   undef SET_LINKAGE
#endif
#ifdef SET_ALLOC
#   undef SET_ALLOC
#endif
#ifdef SET_FREE
#   undef SET_FREE
#endif
#ifdef SET_DECLARE
#   undef SET_DECLARE
#endif
#ifdef SET_DEFINE
#   undef SET_DEFINE
#endif
#ifdef SET_CMP
#   undef SET_CMP
#endif
#ifdef SET_CMP_AS
#   undef SET_CMP_AS
#endif
#ifdef SET_N
#   undef SET_N
#endif
#ifdef SET_T
#   undef SET_T
#endif
