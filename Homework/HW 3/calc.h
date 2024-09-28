#ifndef _CALC_H
#define _CALC_H

#ifndef STACKSIZE
#define STACKSIZE 4
#endif

// OPERATION CONSTANTS
#define ADD 1
#define SUBTRACT 2
#define MULTIPLY 3
#define DIVIDE 4
#define SQRT 5

extern int push(double number);     // Pushes a number onto the stack.
extern void pop();                  // Pops an entry and leave a 0.0 in its place.
extern double get_entry(int flag);  // Returns an element from the stack.

//
// !!!!!!!!!!!!!!!!!! DO NOT TOUCH ANY CODE ABOVE THIS LINE  !!!!!!!!!!!!!!!!!!
// !!!!! YOU WILL NEED TO WRITE THE FUNCTION PROTOTYPES BENEATH THIS LINE !!!!!
//

extern int get_size();  // return number of elements in the stack, 0 if empty

extern double get_tos();  // return element on the top of stack

extern void do_oper(int method);  // Performs the operation on either the top
                                  // two stack entries or the top stack entry.
extern void swap_entry();         // Swaps the top two entries of the stack. Does not return a
                                  // value. If the stack has fewer than 2 entries, do nothing.

extern void rotate_stack();  // Rotates the stack such that the bottom element becomes the
                             // top, topmost element becomes the second topmost and so on.
                             // If the stack has one or fewer entries, do nothing.

#endif
