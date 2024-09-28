/**
 * Assignment-3
 * CSE30 Username: NG Zhe Wee (Derrick)
 * Class: UCSD CSE30-FA21
 */

#include "calc.h"

#include <math.h>

//
// !!!!!!!! DO NOT TOUCH ANY CODE ABOVE THIS LINE (not incl. username) !!!!!!!!
// !!!!!!!!            YOU MAY WRITE STATIC VARIABLES BELOW            !!!!!!!!
//

static double stack[STACKSIZE] = {};
static int stack_size = 0;

/**
 * Push a new value onto the stack.
 *
 * If the stack is full, do not push anything.
 *
 * @return 1 if push is successful, 0 otherwise
 */
int push(double d) {
    if (stack_size >= STACKSIZE) return 0;
    stack[stack_size++] = d;
    return 1;
}

/**
 * Pop the current entry on the top of stack.
 *
 * Do nothing if there nothing on the stack.
 */
void pop() {
    if (stack_size > 0) {
        stack_size--;
    }
}

/**
 * Get an entry from the stack.
 *
 * If passed a postive number, return the oldest stack entry.
 * Else, return the next oldest stack entry (with respect to a prior call).
 *
 * Passing in a positive number should reset the sequence back to the oldest
 * stack entry.
 */
double get_entry(int flag) {
    static int entry_idx = 0;
    if (flag > 0) {
        entry_idx = 0;
    }
    if (entry_idx < stack_size) {
        return stack[entry_idx++];
    } else {
        /**
         * to avoid invalid memory access.
         */
        return 0;
    }
}

/**
 * Returns the number of elements in the stack.
 *
 * @return number of elements in the stack
 */

int get_size() { return stack_size; }

/**
 * Return the element on the top of the stack (the youngest entry).
 *
 * If the stack is empty, return 0.0.
 *
 * @return the element on the top of the stack
 */
double get_tos() {
    if (stack_size == 0) {
        return 0;
    } else {
        return stack[stack_size - 1];
    }
}

/**
 * Do an operation on the top two stack elements.
 *
 * Operation may be one of following: ADD, SUBTRACT, MULTIPLY, DIVIDE, SQRT.
 */
void do_oper(int method) {
    if (ADD == method || SUBTRACT == method || MULTIPLY == method || DIVIDE == method) {
        if (stack_size < 2) {
            // two operands are needed for +, -, *, /
            return;
        }
    } else if (SQRT == method) {
        if (stack_size < 1) {
            // one operand is needed for sqrt
            return;
        }
    } else {
        return;  // invalid method
    }

    // extract a value
    double second = get_tos();
    pop();

    if (method != SQRT) {
        // extract another value
        double first = get_tos();
        pop();

        if (ADD == method) {
            push(first + second);
        } else if (SUBTRACT == method) {
            push(first - second);
        } else if (MULTIPLY == method) {
            push(first * second);
        } else if (DIVIDE == method) {
            push(first / second);
        }
    } else if (method == SQRT) {
        push(sqrt(second));
    }
}

/**
 * Swap the top two stack entries.
 *
 * If the stack has fewer than 2 entries, do nothing.
 */
void swap_entry() {
    if (stack_size >= 2) {
        int idx1 = stack_size - 1;
        int idx2 = stack_size - 2;
        double tmp = stack[idx1];
        stack[idx1] = stack[idx2];
        stack[idx2] = tmp;
    }
}

/**
 * Makes oldest stack entry becomes the youngest,
 * youngest becomes the second youngest,
 * second oldest becomes the oldest, etc.
 */

void rotate_stack() {
    if (stack_size > 0) {
        double tmp = stack[0];
        for (int i = 0; i + 1 < stack_size; i++) {
            stack[i] = stack[i + 1];
        }
        stack[stack_size - 1] = tmp;
    }
}
