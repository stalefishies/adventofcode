#ifndef STRING_HGUARD
#define STRING_HGUARD

#include "common.h"

EXPORT bool   strEq(const String a, const String b);
EXPORT s64    strCmp(const String a, const String b);
EXPORT u64    strHash(const String a);
EXPORT s64    strIndex(const String text, const String key);
EXPORT s64    strCount(const String text, const String key);
EXPORT String strSplit(String *text, const String key);

typedef struct FmtCode {
    u8  type;
    u8  bits;
    u8  fill;

    u32 len_pad;    // required length after padding
    u32 len_max;    // upper bound for length of data

    union {
        u64 u;
        s64 d;
        u8 *s;
    } data;
} FmtCode;

EXPORT const char *fmtParse(FmtCode *codes, us max, const char *fmt, va_list args);

EXPORT String  format(const char *fmt, ...);
EXPORT String vformat(const char *fmt, va_list args);
EXPORT String  vprint(const char *fmt, va_list args);

#endif /* STRING_HGUARD */


#ifdef  AOC_IMPLEMENTATION
#ifndef STRING_IMPLEMENTED
#define STRING_IMPLEMENTED

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

bool strEq(const String a, const String b) {
    if (a.length != b.length) { return false; }
    for (us i = 0; i < a.length; ++i) {
        if (a.data[i] != b.data[i]) { return false; }
    }
    return true;
}

s64 strCmp(const String a, const String b) {
    for (us i = 0;; ++i) {
        s64 cmp = (s64)a.data[i] - (s64)b.data[i];
        if (cmp != 0) {
            return cmp;
        } else if (i == a.length || i == b.length) {
            return (i == b.length) - (i == a.length);
        }
    }
}

// FNV-01a
u64 strHash(const String a) {
    u64 hash = 0xCBF29CE484222325ULL;

    for (us i = 0; i < a.length; ++i) {
        hash ^= a.data[i];
        hash *= 0x100000001B3ULL;
    }
    
    return hash;
}

s64 strIndex(const String text, const String key) {
    s64 max_offset = text.length - key.length;
    for (s64 offset = 0; offset <= max_offset; ++offset) {
        bool found = true;
        for (us i = 0; found && i < key.length; ++i) {
            found = (text.data[offset + i] == key.data[i]);
        }
        if (found) { return offset; }
    }
    
    return -1;
}

s64 strCount(const String text, const String key) {
    s64 max_offset = text.length - key.length;
    s64 count = 0;

    for (s64 offset = 0; offset <= max_offset; ++offset) {
        bool found = true;
        for (us i = 0; found && i < key.length; ++i) {
            found = (text.data[offset + i] == key.data[i]);
        }
        if (found) { ++count; }
    }
    
    return count;
}

String strSplit(String *text, const String key) {
    String first = *text;

    const s64 offset = strIndex(*text, key);
    if (offset < 0) {
        text->data  += text->length;
        text->length = 0;
    } else {
        first.length = offset;
        text->data   += offset + key.length;
        text->length -= offset + key.length;
    }

    return first;
}

EXPORT const char *fmtParse(FmtCode *codes, us max_count, const char *fmt, va_list args) {
    for (us i = 0; i < max_count; ++i) { codes[i] = (FmtCode){0}; }

    us count = 0;
    while (*fmt) {
        if (count == max_count || *fmt == 0) { break; }

        const char *str_begin = fmt;
        for (;;) {
            bool escape = false;
            if (*fmt == '\\') { escape = true; ++fmt; }

            if (*fmt == 0 || (!escape && *fmt == '{')) { break; }
            ++fmt;
        }

        if (fmt > str_begin) {
            codes[count++] = (FmtCode){
                .type    = 'F',
                .len_pad = fmt - str_begin,
                .len_max = fmt - str_begin,
                .data.s  = (u8 *)str_begin
            };
        }

        if (count == max_count || *fmt == 0) { break; }
        if (*fmt++ != '{') { error("Expected '{' to begin format code"); }

        FmtCode *code = &codes[count++];
        bool have_data  = false;
        bool have_width = false;

        for (;;) {
            if      (*fmt == ',') { ++fmt; continue; }
            else if (*fmt == '}') { ++fmt; break; }
            else if (*fmt ==   0) { have_data = true; code->type = 0; break; }

            while (*fmt == ' ') { ++fmt; }

            if (!have_width && *fmt >= '0' && *fmt <= '9') {
                u64 value = 0;

                while (*fmt >= '0' && *fmt <= '9') {
                    value *= 10;
                    value += *fmt - '0';
                    ++fmt;
                }

                code->len_pad = value;
                have_width = true;
            } else if (!code->fill && *fmt == '*') {
                ++fmt;
                if (*fmt >= 0x20) { code->fill = *fmt; ++fmt; }
            } else if (!have_data && *fmt == 'c') {
                code->type = *fmt++;

                code->data.u  = va_arg(args, unsigned);
                code->bits    = 8;
                code->len_max = 1;

                have_data = true;
            } else if (!have_data && *fmt == 's') {
                code->type = *fmt++;

                code->data.s = va_arg(args, u8 *);
                code->len_max = 0;
                for (u8 *at = code->data.s; *at; ++code->len_max, ++at);

                have_data = true;
            } else if (!have_data && *fmt == 'S') {
                code->type = *fmt++;

                String string = va_arg(args, String);
                code->data.s  = string.data;
                code->len_max = string.length;

                have_data = true;
            } else if (!have_data && (*fmt == 'u' || *fmt == 'd'
                || *fmt == 'x' || *fmt == 'b'))
            {
                code->type = *fmt++;
                code->bits = 64;

                if (*fmt == '8') { 
                    code->bits = 8; ++fmt;
                } else if (*fmt == '1' && *(fmt + 1) == '6') {
                    code->bits = 16; ++fmt; ++fmt;
                } else if (*fmt == '3' && *(fmt + 1) == '2') {
                    code->bits = 32; ++fmt; ++fmt;
                } else if (*fmt == '6' && *(fmt + 1) == '4') {
                    code->bits = 64; ++fmt; ++fmt;
                }

                if (code->type == 'd') {
                    if      (code->bits ==  8) { code->data.d =  (s8)va_arg(args, int); }
                    else if (code->bits == 16) { code->data.d = (s16)va_arg(args, int); }
                    else if (code->bits == 32) { code->data.d = (s32)va_arg(args, int); }
                    else                       { code->data.d = (s64)va_arg(args, s64); }
                } else {
                    if      (code->bits ==  8) { code->data.u =  (u8)va_arg(args, unsigned); }
                    else if (code->bits == 16) { code->data.u = (u16)va_arg(args, unsigned); }
                    else if (code->bits == 32) { code->data.u = (u32)va_arg(args, unsigned); }
                    else                       { code->data.u = (u64)va_arg(args, u64);      }
                }

                if (code->type == 'u' || code->type == 'd') {
                    u64 value = (code->type == 'u' ? code->data.u
                        : (code->data.d < 0 ? -code->data.d : code->data.d));

                    code->len_max = 1;
                    if      (value <                   10ULL) { code->len_max +=  1; }
                    else if (value <                  100ULL) { code->len_max +=  2; }
                    else if (value <                 1000ULL) { code->len_max +=  3; }
                    else if (value <                10000ULL) { code->len_max +=  4; }
                    else if (value <               100000ULL) { code->len_max +=  5; }
                    else if (value <              1000000ULL) { code->len_max +=  6; }
                    else if (value <             10000000ULL) { code->len_max +=  7; }
                    else if (value <            100000000ULL) { code->len_max +=  8; }
                    else if (value <           1000000000ULL) { code->len_max +=  9; }
                    else if (value <          10000000000ULL) { code->len_max += 10; }
                    else if (value <         100000000000ULL) { code->len_max += 11; }
                    else if (value <        1000000000000ULL) { code->len_max += 12; }
                    else if (value <       10000000000000ULL) { code->len_max += 13; }
                    else if (value <      100000000000000ULL) { code->len_max += 14; }
                    else if (value <     1000000000000000ULL) { code->len_max += 15; }
                    else if (value <    10000000000000000ULL) { code->len_max += 16; }
                    else if (value <   100000000000000000ULL) { code->len_max += 17; }
                    else if (value <  1000000000000000000ULL) { code->len_max += 18; }
                    else if (value < 10000000000000000000ULL) { code->len_max += 19; }
                    else                                      { code->len_max += 20; }
                } else if (code->type == 'x') {
                    code->len_max = (code->bits + 3) / 4;
                } else {
                    code->len_max = code->bits;
                }

                have_data = true;
            }

            while (*fmt != ',' && *fmt != '}' && *fmt != 0) { ++fmt; }
        }

        if (!have_data) {
            code->type = 'S';

            String string = va_arg(args, String);
            code->data.s  = string.data;
            code->len_max = string.length;

            have_data = true;
        }
    }

    return (*fmt == 0 ? NULL : fmt);
}

String vformat(const char *fmt, va_list args) {
    FmtCode codes[20];
    const char *end = fmtParse(codes, 20, fmt, args);

    us len_max = 0;
    for (us i = 0; codes[i].type && i < 20; ++i) {
        len_max += (codes[i].len_pad > codes[i].len_max ? codes[i].len_pad : codes[i].len_max);
    }
    
    us len_end = 0;
    if (end) { for (const char *at = end; *at; ++len_end, ++at); }
    len_max += len_end;

    u8 *str = alloc(u8, len_max + 1);
    u8 *at  = str;

    for (us i = 0; codes[i].type && i < 20; ++i) {
        FmtCode C = codes[i];
        if (C.fill < 0x20) { C.fill = ' '; }

        switch (C.type) {
            case 'c': {
                if (C.len_pad > 1) {
                    for (us i = 0; i < C.len_pad - 1; ++i) { *at++ = C.fill; }
                }

                *at++ = (u8)C.data.u;
            } break;

            case 's':
            case 'S':
            case 'F': {
                for (us i = 0; i < C.len_max; ++i) {
                    if (C.type == 'F' && i < C.len_max - 1
                        && C.data.s[i] == '\\') { ++i; }
                    *at++ = C.data.s[i];
                }

                if (C.len_pad > C.len_max) {
                    for (us i = 0; i < C.len_pad - C.len_max; ++i) { *at++ = C.fill; }
                }
            } break;

            case 'u':
            case 'd': {
                u32 sgn_length = C.len_max - 1;
                if (C.type == 'd' && C.data.d < 0) { ++sgn_length; }
                if (C.len_pad > sgn_length) {
                    for (us i = 0; i < C.len_pad - sgn_length; ++i) { *at++ = C.fill; }
                }

                if (C.type == 'd' && C.data.d < 0) { *at++ = '-'; }
                u64 value = (C.type == 'u' ? C.data.u
                    : (C.data.d < 0 ? -C.data.d : C.data.d));
                
                at += C.len_max - 1;
                u8 *ptr = at - 1;

                *ptr = '0';
                while (value) { *ptr = '0' + value % 10; value /= 10; --ptr; }
            } break;

            case 'x': {
                if (C.len_pad > C.len_max) {
                    for (us i = 0; i < C.len_pad - C.len_max; ++i) { *at++ = C.fill; }
                }

                u64 value = C.data.u;
                at += C.len_max;
                u8 *ptr = at - 1;

                for (us i = 0; i < C.len_max; ++i) {
                    u64 digit = value & 0xF;
                    *ptr-- = (digit < 0xA ? '0' + digit : 'A' + digit - 0xA);
                    value >>= 4;
                }
            } break;

            case 'b': {
                if (C.len_pad > C.len_max) {
                    for (us i = 0; i < C.len_pad - C.len_max; ++i) { *at++ = C.fill; }
                }

                u64 value = C.data.u;
                at += C.len_max;
                u8 *ptr = at - 1;

                for (us i = 0; i < C.len_max / 8; ++i) {
                    for (us j = 0; j < 8; ++j) {
                        u64 digit = value & (0x1 << j);
                        *ptr-- = (digit ? '1' : '0');
                    }
                    value >>= 1;
                }
            } break;

            default: error("Unknown format code type '{c}'", C.type);
        }
    }

    us length = at - str;
    for (us i = length; i < len_max + 1; ++i) { str[i] = '\0'; }
    return (String){ .data = str, .length = length };
}

String format(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    String string = vformat(fmt, args);
    va_end(args);
    return string;
}

String vprint(const char *fmt, va_list args) {
    String string = vformat(fmt, args);

    if (string.data[string.length - 1] == '\\') {
        printf("%.*s", (int)string.length - 1, string.data);
    } else {
        printf("%.*s\n", (int)string.length, string.data);
    }

    return string;
}

String print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    String string = vprint(fmt, args);
    va_end(args);
    return string;
}

void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprint(fmt, args);
    va_end(args);
    exit(1);
}

#endif /* STRING_IMPLEMENTED */
#endif /* AOC_IMPLEMENTATION */
