/*
 * CSE30 FA21 HW5
 * CSE30 username: cs30fa21cmi 
 */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "globals.h"

/* Prints the usage message */
void printUsage() {
    //   - simple one line of code
    //   - string can be found in globals.h
    printf("%s", USAGE);
}

/*
 * Executes the code, accepting a font file, populating the fontBuffer array
 * with the corresponding cBanners, and then printing the given string using the
 * cBanners. A -u flag should print the usage message. A -w flag followed by an
 * int should change the width of the displayBuffer. A -h followed by an int
 * should change the height of the displayBuffer. A -f flag followed by a char
 * should replace the background symbol of the displayBuffer accordingly.
 */
int main(int argc, char** argv) {
    //   - handle options using getopt
    char fillSymbol = ' ';
    char* font_file = 0;
    {
        int result = 0;
        while ((result = getopt(argc, argv, "uw:h:f:")) != -1) {
            switch (result) {
                case 'u':
                    printUsage();
                    return 0;
                    break;
                case 'w':
                    if (optarg) {
                        displayWidth = atoi(optarg);
                        if (displayWidth > MAXWIDTH) {
                            displayWidth = MAXWIDTH;
                        }
                    }
                    break;
                case 'h':
                    if (optarg) {
                        displayHeight = atoi(optarg);
                        if (displayHeight > MAXHEIGHT) {
                            displayHeight = MAXHEIGHT;
                        }
                    }
                    break;
                case 'f':
                    if (optarg) {
                        fillSymbol = optarg[0];
                    }
                    break;
                default:
                    break;
            }
        }
        if (optind < argc) {
            font_file = argv[optind];
        }
    }

    //   - how can you read in the cBanners from the font file?
    //   - how should displayBuffer be initialized?
    //   - how should you limit the height and width of displayBuffer if provided
    //   in the options?
    //   - how can you write the input into the displayBuffer using cBanners?
    //   - what are the spacing rules between words and between cBanners within a
    //   word?
    //   - how can you print the displayBuffer?
    //   - return 0

    if (optind + 1 >= argc) return 0;

    readFontBuffer(font_file);
    fillDisplayBuffer(fillSymbol);
    int pos = 0;
    for (int i = optind + 1; i < argc; i++) {
        char* input_text = argv[i];
        int len = strlen(input_text);
        int j = 0;
        while (j < len) {
            if (input_text[j] == ':' && j + 1 < len && input_text[j + 1] == ')') {
                pos = copyCBanner(SMILEY, pos);
                j += 2;
            } else {
                pos = copyCBanner(input_text[j], pos);
                j++;
            }
        }

        if (i + 1 < argc) {
            pos = copyCBanner(' ', pos);
        }
    }
    printDisplayBuffer();
    return 0;
}
