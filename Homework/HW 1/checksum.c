
/**
 * Assignment-1
 * CSE30 Username: NG Zhe Wee (Derrick)
 * Class: UCSD CSE30-FA21
 */
// Your program goes below

#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned char sum = 255;
    int c = EOF;
    while ((c = getchar()) != EOF)
    {
        putchar(c);
        sum += (unsigned char)c;
    }
    printf("%d\n", (int)sum);
    return 0;
}