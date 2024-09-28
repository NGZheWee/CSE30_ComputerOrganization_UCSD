#ifndef _GLOBALS_H
#define _GLOBALS_H

#define MAXFONTSIZE 10 * 4096
#define MAXFONTLINESIZE 32
#define MAXNUMCBANNERS 200
#define MAXWIDTH 256
#define MAXHEIGHT 24
#define FONTDELIM '#'
#define SPACING 2
#define SMILEY ('~' + 1)
#define FIRSTCHAR ' '
#define USAGE                                                                                           \
    "Usage: ./bannerbuild [-u] [-h <height>] [-w <width>] [-f <fillSymbol>] <font file> <input text>\n" \
    "  [-u]: Print the usage\n"                                                                         \
    "  [-h <height>]: Specify the height of the banner build\n"                                         \
    "  [-w <width>]: Specify the width of the banner build\n"                                           \
    "  [-f <fillSymbol>]: Specify the fillSymbol of the banner build\n"                                 \
    "  <font file>: The path of a font file\n"                                                          \
    "  <input text>: The words you want to appear in the output\n"

extern int readFontBuffer(const char *);
extern void printCBanner(const char);
extern int copyCBanner(const char, int);
extern void fillDisplayBuffer(char);
extern void printDisplayBuffer();

extern char fontBuffer[MAXFONTSIZE];
extern char *cBannerLookup[MAXNUMCBANNERS];
extern int cBannerWidth;
extern char displayBuffer[MAXHEIGHT][MAXWIDTH];
extern int displayHeight;
extern int displayWidth;

#endif
