#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 256

int is_space(char c);

int main(int argc, char *argv[]) {
    FILE *fin, *fout;
    char str[MAX_LEN];
    int i = 0;
    int len = 0;
    int j = 0;

    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    // Read the file one line at a time
    while (fgets(str, MAX_LEN, fin)) {
        // Replace multiple whitespace chars with a single space ' '
        len = strlen(str);
        for (i = 0; i < len;) {
            if (is_space(str[i])) {
                str[i] = ' ';
                // the remaining consecutive spaces adjacent to current space.
                int spaces = 0;
                j = i + 1;
                while (j < len && is_space(str[j])) {
                    spaces++;
                    j++;
                }
                if (spaces > 0) {
                    // if there are consecutive spaces, preserve only one.
                    for (j = i; j + spaces < len; j++) {
                        str[j] = str[j + spaces];
                    }
                    str[len - spaces] = 0;
                    len -= spaces;
                } else {
                    i++;
                }

            } else if (str[i] == '\n') {
                if (i > 0 && is_space(str[i - 1])) {
                    // if current char is '\n' and the previous is space, remove the previous space.
                    str[i - 1] = '\n';
                    str[i] = 0;
                    len--;
                }
                break;
            } else {
                i++;
            }
        }
        for (i = 0; i < len; i++) {
            fputc(str[i], fout);
        }
    }

    // Clean up gracefully
    fclose(fin);
    fclose(fout);
    return 0;
}

int is_space(char c) { return (c == ' ' || c == '\f' || c == '\r' || c == '\t' || c == '\v') ? 1 : 0; }
