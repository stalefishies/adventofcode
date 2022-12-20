#include "aoc.h"

typedef struct Dir {
    String name;
    u64    size;
    struct Dir *parent;
    struct Dir *next;
    struct Dir *child;
} Dir;

void solver(void) {
    // skip first line of input
    strSplit(&input, strLiteral("\n"));

    Dir root = (Dir){ .name = strLiteral("/") };
    Dir *at  = &root;

    PARSE_INPUT() {
        if (pAccept("$ cd ..\n")) {
            if (!at->parent) { error("Tried to run 'cd ..' from root"); }
            at->parent->size += at->size;
            at = at->parent;
        } else if (pAccept("$ cd ")) {
            String name = pSplit("\n");

            Dir *child = at->child;
            while (child) {
                if (strEq(name, child->name)) { break; }
                child = child->next;
            }

            if (!child) { error("Could not cd to directory {}", name); }
            at = child;
        } else {
            pAssert("$ ls\n");
            Dir *prev = NULL;

            while (pPeek() != '$' && pPeek() != 0) {
                u64 size = pNum();
                if (size > 0) {
                    at->size += size;
                    pSplit("\n");
                } else {
                    Dir *dir = alloc(Dir, 1);
                    if (!at->child) { at->child = dir; }

                    pAssert("dir ");
                    dir->name = pSplit("\n");

                    dir->parent = at;
                    dir->next   = NULL;
                    dir->child  = NULL;

                    if (prev) { prev->next = dir; }
                    prev = dir;
                }
            }
        }
    }

    while (at->parent) {
        at->parent->size += at->size;
        at = at->parent;
    }

    if (at != &root) { error("Directory traversal ended at {}", at->name); }

    u64 part_one = 0;
    u64 part_two = root.size;
    u64 required = root.size - 40000000;

    do {
        u64 size = at->size;
        if (size <= 100000) { part_one += size; }
        if (size >= required && size < part_two) { part_two = size; }

        if (at->child) {
            at = at->child;
        } else {
            while (!at->next && at->parent) { at = at->parent; }
            if (at->next) { at = at->next; }
        }
    } while (at != &root);

    solve("{u}", part_one);
    solve("{u}", part_two);
}
