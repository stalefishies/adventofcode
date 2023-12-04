#include "aoc.h"

// TODO:
//  print fixed-size string from ptr (not String)
//  ARR_EQ_AS

typedef union Name {
    u8  str[4];
    u32 val;
} Name;

#define ARR_T Name
#define ARR_N name
//#define ARR_EQ_AS(n) n.val
#include "arr.h"

typedef struct Monkey {
    u64 value;
    u16 op;
    u16 parent;
    u16 child[2];
} Monkey;

#define ROOT 0x746F6F72U
#define HUMN 0x6E6D7568U
#define OP_LIT 0xFFFB
#define OP_ADD 0xFFFC
#define OP_SUB 0xFFFD
#define OP_MUL 0xFFFE
#define OP_DIV 0xFFFF
#define UNEVALUATED 0x8000000000000000ULL

void solver(void) {
//    input = strLiteral(
//"root: pppw + sjmn\n"
//"dbpl: 5\n"
//"cczh: sllz + lgvd\n"
//"zczc: 2\n"
//"ptdq: humn - dvpt\n"
//"dvpt: 3\n"
//"lfqf: 4\n"
//"humn: 5\n"
//"ljgn: 2\n"
//"sjmn: drzm * dbpl\n"
//"sllz: 4\n"
//"pppw: cczh / lfqf\n"
//"lgvd: ljgn * ptdq\n"
//"drzm: hmdt - zczc\n"
//"hmdt: 32\n"
//    );

    // Create name array
    Name *names = NULL;
    PARSE_INPUT("\n") {
        Name name;
        name.str[0] = pChar();
        name.str[1] = pChar();
        name.str[2] = pChar();
        name.str[3] = pChar();
        namePush(&names, name);
    }
    us count = nameLen(names);

    // Save special monkey indices
    us index_root = 0;
    us index_humn = 0;
    for (us i = 0; i < count && (index_root == 0 || index_humn == 0); ++i) {
        if (names[i].val == ROOT) {
            index_root = i;
        } else if (names[i].val == HUMN) {
            index_humn = i;
        }
    }

    // Create monkey array
    us index = 0;
    Monkey *monkeys = alloc(Monkey, count);
    PARSE_INPUT("\n") {
        Monkey *monkey = &monkeys[index++];

        pString(4);
        pAssert(": ");

        u32 value = pNum();
        if (value) {
            monkey->op    = OP_LIT;
            monkey->value = value;
        } else {
            Name name;

            name.str[0] = pChar();
            name.str[1] = pChar();
            name.str[2] = pChar();
            name.str[3] = pChar();
            for (us i = 0; i < nameLen(names); ++i) {
                if (name.val == names[i].val) {
                    monkey->child[0] = i;
                    break;
                }
            }
            pAssert(" ");
            u8 op = pChar();
            if      (op == '+') { monkey->op = OP_ADD; }
            else if (op == '-') { monkey->op = OP_SUB; }
            else if (op == '*') { monkey->op = OP_MUL; }
            else if (op == '/') { monkey->op = OP_DIV; }
            else { error("invalid op {c}", op); }
            pAssert(" ");

            name.str[0] = pChar();
            name.str[1] = pChar();
            name.str[2] = pChar();
            name.str[3] = pChar();

            for (us i = 0; i < nameLen(names); ++i) {
                if (name.val == names[i].val) {
                    monkey->child[1] = i;
                    break;
                }
            }

            monkey->value = UNEVALUATED;
        }
    }

    // Set parent backreferences
    for (us i = 0; i < count; ++i) {
        if (monkeys[i].op == OP_LIT) { continue; }
        monkeys[monkeys[i].child[0]].parent = i;
        monkeys[monkeys[i].child[1]].parent = i;
    }
    monkeys[index_root].parent = -1;

    // Bake in results that don't depend on 'humn'
    for (bool changed = true; changed;) {
        changed = false;

        for (us i = 0; i < count; ++i) {
            Monkey *monkey = &monkeys[i];

            if (monkey->value != UNEVALUATED
                || monkey->child[0] == index_humn
                || monkey->child[1] == index_humn)
            { continue; }

            u64 args[2];
            args[0] = monkeys[monkey->child[0]].value;
            if (args[0] == UNEVALUATED) { continue; }
            args[1] = monkeys[monkey->child[1]].value;
            if (args[1] == UNEVALUATED) { continue; }

            if      (monkey->op == OP_ADD) { monkey->value = args[0] + args[1]; }
            else if (monkey->op == OP_SUB) { monkey->value = args[0] - args[1]; }
            else if (monkey->op == OP_MUL) { monkey->value = args[0] * args[1]; }
            else                           { monkey->value = args[0] / args[1]; }

            monkey->op = OP_LIT;
            changed = true;
        }
    }

    // Evaluate as normal for part one
    for (bool done = false; !done;) {
        for (us i = 0; i < count; ++i) {
            Monkey *monkey = &monkeys[i];
            if (monkey->value != UNEVALUATED) { continue; }

            u64 args[2];
            args[0] = monkeys[monkey->child[0]].value;
            if (args[0] == UNEVALUATED) { continue; }
            args[1] = monkeys[monkey->child[1]].value;
            if (args[1] == UNEVALUATED) { continue; }

            if      (monkey->op == OP_ADD) { monkey->value = args[0] + args[1]; }
            else if (monkey->op == OP_SUB) { monkey->value = args[0] - args[1]; }
            else if (monkey->op == OP_MUL) { monkey->value = args[0] * args[1]; }
            else                           { monkey->value = args[0] / args[1]; }

            if (i == index_root) {
                solve("{d}", monkey->value);
                done = true;
                break;
            }
        }
    }

    // Reset part one evaluation
    for (us i = 0; i < count; ++i) {
        if (monkeys[i].op != OP_LIT) { monkeys[i].value = UNEVALUATED; }
    }

    // Set root value
    Monkey *root = &monkeys[index_root];
    root->op = OP_LIT;
    if (monkeys[root->child[0]].value == UNEVALUATED) {
        root->value = monkeys[root->child[1]].value;
        monkeys[root->child[0]].value = monkeys[root->child[1]].value;
    } else {
        root->value = monkeys[root->child[0]].value;
        monkeys[root->child[1]].value = monkeys[root->child[0]].value;
    }

    // Unset humn value
    monkeys[index_humn].value = UNEVALUATED;

    // Evaluate in reverse for part two
    for (bool done = false; !done;) {
        for (us i = 0; i < count; ++i) {
            Monkey *monkey = &monkeys[i];
            if (monkey->value != UNEVALUATED) { continue; }

            Monkey parent = monkeys[monkey->parent];
            if (parent.value == UNEVALUATED) { continue; }

            u64 result = monkeys[monkeys[i].parent].value;
            us child = (i == parent.child[1]);

            if (child == 0) {
                u64 arg = monkeys[parent.child[1]].value;

                if      (parent.op == OP_ADD) { monkey->value = result - arg; }
                else if (parent.op == OP_SUB) { monkey->value = result + arg; }
                else if (parent.op == OP_MUL) { monkey->value = result / arg; }
                else                          { monkey->value = result * arg; }
            } else {
                u64 arg = monkeys[parent.child[0]].value;

                if      (parent.op == OP_ADD) { monkey->value = result - arg; }
                else if (parent.op == OP_SUB) { monkey->value = arg - result; }
                else if (parent.op == OP_MUL) { monkey->value = result / arg; }
                else                          { monkey->value = arg / result; }
            }

            if (i == index_humn) {
                solve("{d}", monkey->value);
                done = true;
                break;
            }
        }
    }
}
