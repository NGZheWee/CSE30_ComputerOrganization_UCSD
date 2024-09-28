/*
 * CSE30 FA21 HW7
 * CSE30 username: NG Zhe Wee (Derrick)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#define BFSZ 4096

extern char encrypt(char inchar, char key);
extern char decrypt(char inchar, char key);
void usage(char **argv);
void parse_opts(int argc, char *argv[], bool *bdecrypt, bool *bencrypt, char bookefile[BFSZ], int *offset, char filename[BFSZ]);
void my_write(char *buf, size_t buf_size, FILE *fp);

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        usage(argv);
        return EXIT_FAILURE;
    }
    else
    {
        bool bdecrypt = false;
        bool bencrypt = false;
        int offset = 0;
        char bookefile[BFSZ] = {0};
        char filename[BFSZ] = {0};
        //Process option flags (using getopt()).
        parse_opts(argc, argv, &bdecrypt, &bencrypt, bookefile, &offset, filename);

        //Open the bookfile for reading and obtain its size.
        FILE *book_fp = fopen(bookefile, "rb");
        if (!book_fp)
        {
            fprintf(stderr, "Unable to open book file: %s\n", bookefile);
            exit(EXIT_FAILURE);
        }
        struct stat stat;
        fstat(fileno(book_fp), &stat);
        size_t book_size = stat.st_size;
        //Create a dynamically allocated array of chars with the size of the bookfile
        char *book_content = (char *)calloc(book_size, 1);
        size_t current_readings = 0;
        size_t total_readings = 0;
        size_t left_readings = book_size;
        //Read the bookfile data into the array
        while ((current_readings = fread(book_content + total_readings, 1, left_readings, book_fp)))
        {
            total_readings += current_readings;
            left_readings -= current_readings;
        }
        fclose(book_fp);
        //Open the encryption_file to write/read
        FILE *using_file = NULL;
        if (bencrypt)
        {
            using_file = fopen(filename, "wb");
        }
        else if (bdecrypt)
        {
            using_file = fopen(filename, "rb");
        }
        if (!using_file)
        {
            fprintf(stderr, "Unable to open file: %s\n", filename);
            exit(EXIT_FAILURE);
        }
        //Begin processing lines to either encrypt or decrypt
        if (bencrypt)
        {
            char line[BFSZ] = {0};
            while (fgets(line, BFSZ, stdin))
            {
                int total_len = strlen(line);
                for (int i = 0; i < total_len; i++, offset++)
                {
                    offset %= book_size;
                    line[i] = encrypt(line[i], book_content[offset]);
                }
                my_write(line, total_len, using_file);
            }
        }
        else
        {
            char buf[BFSZ] = {0};
            size_t current_readings = 0;
            while ((current_readings = fread(buf, 1, BFSZ, using_file)) > 0)
            {
                for (size_t i = 0; i < current_readings; i++, offset++)
                {
                    offset %= book_size;
                    buf[i] = decrypt(buf[i], book_content[offset]);
                }
                my_write(buf, current_readings, stdout);
            }
        }
        fclose(using_file);
        free(book_content);
    }
    return 0;
}

void my_write(char *buf, size_t buf_size, FILE *fp)
{
    size_t left_len = buf_size;
    size_t current_writings = 0;
    size_t total_writings = 0;
    while (left_len > 0)
    {
        current_writings = fwrite(buf + total_writings, 1, left_len, fp);
        if (current_writings == 0)
        {
            break;
        }
        total_writings += current_writings;
        left_len -= current_writings;
    }
}

void usage(char **argv)
{
    fprintf(stderr, "Usage: %s (-d|-e) -b <bookfile> [-o <offset>] <filename>\n", *argv);
    return;
}

void parse_opts(int argc, char *argv[], bool *bdecrypt, bool *bencrypt, char bookefile[BFSZ], int *offset, char filename[BFSZ])
{
    int opt;

    opterr = 0;
    *bdecrypt = false;
    *bencrypt = false;
    bookefile[0] = 0;
    filename[0] = 0;
    while ((opt = getopt(argc, argv, "deb:o:")) != -1)
    {
        switch (opt)
        {
        case 'd':
            *bdecrypt = true;
            break;
        case 'e':
            *bencrypt = true;
            break;
        case 'o':
            *offset = atoi(optarg);
            break;
        case 'b':
            strncpy(bookefile, optarg, BFSZ);
            break;
        case '?':
            fprintf(stderr, "%s: option requires an argument -- '%c'\n", argv[0], optopt);
            exit(EXIT_FAILURE);
            break;
        default:
            break;
        }
    }
    if (optind < argc)
    {
        strncpy(filename, argv[optind], BFSZ);
    }
    if (bookefile[0] == 0)
    {
        fprintf(stderr, "Unable to open book file: (null)\n");
        exit(EXIT_FAILURE);
    }
    else if (filename[0] == 0)
    {
        fprintf(stderr, "Unable to open file: (null)\n");
        exit(EXIT_FAILURE);
    }
    else if (!bdecrypt && !bencrypt)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }
}
