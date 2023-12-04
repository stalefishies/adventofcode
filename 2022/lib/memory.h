#ifndef MEMORY_HGUARD
#define MEMORY_HGUARD

#include "common.h"

typedef struct AllocBlock {
    struct AllocBlock *prev;
    char *end;
    char memory[];
} AllocBlock;

typedef struct AllocCtx {
    AllocBlock *block;
    char *head;
    size_t used;
    size_t total;
} AllocCtx;

EXPORT void memSetCtx(AllocCtx *ctx);
EXPORT void *memAlloc(size_t size);
EXPORT void memFreeAll(void);
#define alloc(type, num) ((type *)memAlloc((num) * sizeof(type)))

#endif /* MEMORY_HGUARD */


#ifdef  AOC_IMPLEMENTATION
#ifndef MEMORY_IMPLEMENTED
#define MEMORY_IMPLEMENTED

#include <stdlib.h>
#include "common.h"

#define BLOCK_SIZE 65536

static AllocCtx *alloc_ctx;
void memSetCtx(AllocCtx *ctx) { alloc_ctx = ctx; }

#include <stdio.h>
void *memAlloc(size_t size) {
    AllocCtx *ctx = alloc_ctx;
    if (!ctx) {
        fprintf(stderr, "no global allocator set\n");
        exit(1);
    }

    void *allocation = NULL;

    if (ctx->block && ctx->head + size <= ctx->block->end) {
        allocation = ctx->head;
        ctx->head += size;
        ctx->used += size;
    } else {
        size_t block_size = ((sizeof(AllocBlock) + size + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;

        AllocBlock *block = malloc(block_size);
        if (!block) { error("malloc returned NULL"); }

        block->prev = ctx->block;
        block->end  = (char *)block + block_size;
        ctx->used   = (ctx->block ? ctx->used  : 0) + size;
        ctx->total  = (ctx->block ? ctx->total : 0) + block_size;
        ctx->block  = block;

        allocation = block->memory;
        ctx->head  = block->memory + size;
    }

    uintptr_t align = (uintptr_t)ctx->head % 8;
    ctx->head += align ? 0 : 8 - align;
    return allocation;
}

void memFreeAll(void) {
    AllocBlock *block = alloc_ctx->block;

    while (block) {
        AllocBlock *prev = block->prev;
        free(block);
        block = prev;
    }

    alloc_ctx->block = NULL;
    alloc_ctx->head  = NULL;
    alloc_ctx->used  = 0;
    alloc_ctx->total = 0;
}

#endif /* MEMORY_IMPLEMENTED */
#endif /* AOC_IMPLEMENTATION */
