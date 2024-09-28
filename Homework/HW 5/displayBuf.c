#include <stdio.h>
#include <string.h>

#include "globals.h"

char displayBuffer[MAXHEIGHT][MAXWIDTH];
int displayHeight = MAXHEIGHT;
int displayWidth = MAXWIDTH;

/*
 * Looks up the specified char's cBanner in fontBuffer, then copies the
 * cBanner into the displayBuffer starting at column xPos. Returns the
 * column where the next cBanner should start.
 */
int copyCBanner(const char c, int xPos) {
    //   - how can you get char c's cBanner?
    //   - how can you copy the cBanner into the displayBuffer?
    //   - is there anything to be careful about when copying?
    //   - this should return the xPos (column) of the next cBanner;
    //     what are the inter-cBanner spacing rules?

    if (xPos < displayWidth) {
        char *begin = cBannerLookup[c - FIRSTCHAR];
        int h = 0;
        int running = 1;
        while (running && h < displayHeight) {
            int w = 0;
            do {
                if (*(begin) == '\n') {
                    if (w == 0) {
                        running = 0;
                    }
                    begin++;
                    break;
                } else {
                    if (xPos + w < MAXWIDTH) {
                        displayBuffer[h][xPos + w] = *begin;
                    }
                }
                begin++;
                w++;
            } while (1);
            h++;
        }
    }

    return xPos + cBannerWidth + SPACING;
}

/* Fills every byte of displayBuffer with the specified char */
void fillDisplayBuffer(const char c) {
    for (int h = 0; h < displayHeight; h++) {
        for (int w = 0; w < displayWidth; w++) {
            displayBuffer[h][w] = c;
        }
    }
}

/* Prints the entire displayBuffer to stdout */
void printDisplayBuffer() {
    for (int h = 0; h < displayHeight; h++) {
        for (int w = 0; w < displayWidth; w++) {
            printf("%c", displayBuffer[h][w]);
        }
        printf("\n");
    }
}
