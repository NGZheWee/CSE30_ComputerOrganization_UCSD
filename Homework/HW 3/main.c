/**
 * Assignment-3
 * CSE30 Username: NG Zhe Wee (Derrick)
 * Class: UCSD CSE30-FA21
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"

#define BUFSZ 1024

void print_help() {
    printf("Commands:\n");
    printf("\th   Print this help message\n");
    printf("\t[#] Push number onto the stack\n");
    printf("\t+   Add youngest stack element and prior stack element\n");
    printf("\t-   Subtract youngest stack element from prior stack element\n");
    printf("\t*   Multiply youngest stack element and prior stack element\n");
    printf("\t/   Divide youngest stack element into prior stack element\n");
    printf("\tT   Square root of the youngest stack element\n");
    printf("\tc   Clear the value of the youngest stack element\n");
    printf("\ts   Swap the top two stack elements\n");
    printf("\tr   Rotate the stack (oldest becomes youngest, ...)\n");
    printf("\tp   Print the stack, oldest to youngest\n");
    printf("\tq   Quit\n");
}

int main(int argc, char *argv[]) {
    char buf[BUFSZ];

    printf(">>> ");
    // Loop fgets to read input (buf) from stdin
    while (fgets(buf, BUFSZ, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = 0;  // Remove new line char from input (buf)
        char cmd;
        double number;

        //
        // !!!!!!!!!!!!!!!!!! DO NOT TOUCH ANY CODE ABOVE THIS LINE
        // !!!!!!!!!!!!!!!!!!
        //

        if (sscanf(buf, "%lf", &number) == 1)  // If the input is a number...
        {
            if (!push(number)) {
                printf("Stack is full, %s ignored\n", buf);
            }
        } else if (sscanf(buf, "%c", &cmd) == 1)  // If the input is a char...
        {
            int show_top = 1;  // some command will not show the top finally.

            switch (cmd) {
                case ('h'):
                    print_help();
                    show_top = 0;
                    break;
                case ('+'):
                    do_oper(ADD);
                    break;
                case ('-'):
                    do_oper(SUBTRACT);
                    break;
                case ('*'):
                    do_oper(MULTIPLY);
                    break;
                case ('/'):
                    do_oper(DIVIDE);
                    break;
                case ('T'):
                    do_oper(SQRT);
                    break;
                case ('c'):
                    pop();
                    break;
                case ('s'):
                    swap_entry();
                    break;
                case ('r'):
                    rotate_stack();
                    break;
                case ('p'):
                    for (int i = 0; i < get_size(); i++) {
                        printf("%lf\n", get_entry(i == 0 ? 1 : 0));
                    }
                    show_top = 0;
                    break;
                case ('q'):
                    return 0;
                default:
                    show_top = 0;
                    printf("Unknown command '%s'\n", buf);
                    break;
            }
            if (show_top) {
                // print the top of the stack
                printf("%lf\n", get_tos());
            }
        }

        //
        // !!!!!!!!!!!!!!!!!! DO NOT TOUCH ANY CODE BELOW THIS LINE
        // !!!!!!!!!!!!!!!!!!
        //

        else  // If the input is malformed (ex: newline)
        {
            printf("Bad input line '%s'\n", buf);
        }
        printf(">>> ");
    }
}
