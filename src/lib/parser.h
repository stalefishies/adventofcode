#ifndef PARSER_HGUARD
#define PARSER_HGUARD

#include "common.h"

EXPORT bool   pDoneCtx      (String *ctx);
EXPORT bool   pAcceptCtx    (String *ctx, const String key);
EXPORT void   pAssertCtx    (String *ctx, const String key);
EXPORT u8     pPeekCtx      (String *ctx);
EXPORT u8     pCharCtx      (String *ctx);
EXPORT String pStringCtx    (String *ctx, us length);
EXPORT String pSplitCtx     (String *ctx, const String key);
EXPORT s64    pNumCtx       (String *ctx);
EXPORT bool   parseSplitCtx (String *ctx, String *split_ctx, const String key);

#define PARSE_STRING(str) for (String _p_ctx = (str); _p_ctx.length > 0; )
#define PARSE_SPLIT(lit)                            \
    for (String _p_ctx_##__LINE__ = _p_ctx;         \
        parseSplitCtx(&_p_ctx, &_p_ctx_##__LINE__,  \
            strLiteral(lit));                       \
    )
#define PARSE_INPUT(lit) PARSE_STRING(input) PARSE_SPLIT("" lit)

#define pGet()          (_p_ctx)
#define pSet(str)       (_p_ctx = str)
#define pLen()          (_p_ctx.length)
#define pDone()         pDoneCtx    (&_p_ctx)
#define pAccept(lit)    pAcceptCtx  (&_p_ctx, strLiteral(lit))
#define pAssert(lit)    pAssertCtx  (&_p_ctx, strLiteral(lit))
#define pPeek()         pPeekCtx    (&_p_ctx)
#define pChar()         pCharCtx    (&_p_ctx)
#define pString(len)    pStringCtx  (&_p_ctx, len)
#define pSplit(lit)     pSplitCtx   (&_p_ctx, strLiteral(lit))
#define pNum()          pNumCtx     (&_p_ctx)

#endif /* PARSER_HGUARD */


#ifdef  AOC_IMPLEMENTATION
#ifndef PARSER_IMPLEMENTED
#define PARSER_IMPLEMENTED

#include "string.h"

bool pDoneCtx(String *ctx) { return ctx->length == 0; }

bool pAcceptCtx(String *ctx, const String key) {
    if (key.length > ctx->length) { return false; }

    for (us i = 0; i < key.length; ++i) {
        if (ctx->data[i] != key.data[i]) { return false; }
    }

    ctx->length -= key.length;
    ctx->data   += key.length;
    return true;
}

void pAssertCtx(String *ctx, const String key) {
    if (!pAcceptCtx(ctx, key)) { error("parse assertion failure"); }
}

u8 pPeekCtx(String *ctx) {
    if (ctx->length == 0) { return 0; }
    return *ctx->data;
}

u8 pCharCtx(String *ctx) {
    if (ctx->length == 0) { return 0; }
    --ctx->length;
    return *ctx->data++;
}

String pStringCtx(String *ctx, us length) {
    if (ctx->length < length) { length = ctx->length; }
    
    String ret   = *ctx;
    ret.length   = length;
    ctx->length -= length;
    ctx->data   += length;
    return ret;
}

String pSplitCtx(String *ctx, const String key) {
    String split = *ctx;

    s64 offset = strIndex(*ctx, key);
    if (offset < 0) {
        ctx->data  += ctx->length;
        ctx->length = 0;
    } else {
        split.length = offset;
        ctx->data   += offset + key.length;
        ctx->length -= offset + key.length;
    }

    return split;
}

s64 pNumCtx(String *ctx) {
    if (ctx->length == 0) { return 0; }
    u8 *at = ctx->data;
    u8 *hi = ctx->data + ctx->length;

    s64 sign = 1;
    if (*at == '-') {
        sign = -1;
        ++at;
    } else if (*at == '+') {
        ++at;
    }

    s64 value = 0;
    while (at < hi && *at >= '0' && *at <= '9') {
        value *= 10;
        value += *at++ - '0';
    }

    ctx->length = hi - at;
    ctx->data   = at;
    return sign * value;
}

bool parseSplitCtx(String *ctx, String *split_ctx, const String key) {
    if (key.length == 0) { return ctx->length > 0; }
    ctx->data = split_ctx->data;

    if (split_ctx->length == 0) {
        ctx->length = 0;
        return false;
    }

    s64 offset = strIndex(*split_ctx, key);
    if (offset < 0) {
        ctx->length       = split_ctx->length;
        split_ctx->data  += split_ctx->length;
        split_ctx->length = 0;
    } else {
        ctx->length        = offset;
        split_ctx->length -= offset + key.length;
        split_ctx->data   += offset + key.length;
    }

    return true;
}

#endif /* PARSER_IMPLEMENTED */
#endif /* AOC_IMPLEMENTATION */
