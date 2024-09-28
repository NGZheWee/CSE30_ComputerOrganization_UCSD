#define _CGUARD_
#include <stdio.h>

#include "encrypter.h"
#define BUFSZ 1024

extern void encryptdelete(void);
extern int setup(int argc, char *argv[], int *mode, FILE **book, FILE **FPInput, FILE **FPoutput);

char encrypt(char, char);
char decrypt(char, char);
int main(int argc, char **argv) {
    int mode = 0;
    char iobuf[BUFSZ] = {0};
    char bookbuf[BUFSZ] = {0};
    FILE *FPBook = 0;
    FILE *FPInput = 0;
    FILE *FPoutput = 0;
    int readcnt = 0;
    int pos = 0;
    int toread = 0;
    int bytes = 0;
    int res = 0;
    if ((res = setup(argc, argv, &mode, &FPBook, &FPInput, &FPoutput)) == EXIT_OK) {
        while ((readcnt = fread(iobuf, 1, BUFSZ, FPInput)) > 0) {
            pos = 0;
            toread = readcnt;
            while ((bytes = fread(&bookbuf[pos], 1, toread, FPBook)) > 0) {
                if ((pos = pos + bytes) == readcnt) {
                    break;
                }
                toread = toread - bytes;
            }
            if (bytes == 0) {
                fprintf(stderr, "Bookfile is too short for message\n");
                res = EXIT_FAIL;
                break;
            }

            for (int j = 0; j < readcnt; j++) {
                if (mode == ENCRYPT_MODE) {
                    iobuf[j] = encrypt(iobuf[j], bookbuf[j]);
                } else {
                    iobuf[j] = decrypt(iobuf[j], bookbuf[j]);
                }
            }
            if ((int)fwrite(iobuf, 1, readcnt, FPoutput) != readcnt) {
                fprintf(stderr, "Write failed on FPoutput\n");
                res = EXIT_FAIL;
                break;
            }
        }
    }

    if (FPBook) {
        fclose(FPBook);
    }
    if (FPInput) {
        fclose(FPInput);
    }
    if (FPoutput) {
        fclose(FPoutput);
    }

    if (res != EXIT_OK && mode == ENCRYPT_MODE) {
        encryptdelete();
    }

    return res;
}