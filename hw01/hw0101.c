#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mystring.h"

int main(int argc, char const *argv[])
{
    char *a = "the string, of the iopoipooioopk";
    char aa[] = "the string,   of the iopoipooioopk";
    char bb[] = "the string,   of the iopoipooioopk";
    char *s2 = ", ";

    printf("%s\n", strchr(a, 'j'));
    printf("%s\n", mystrchr(a, 'j'));
    printf("%s\n", strrchr(a, 'j'));
    printf("%s\n", mystrrchr(a, 'j'));
    printf("%d\n", strspn(a, s2));
    printf("%d\n", mystrspn(a, s2));
    printf("%d\n", strcspn(a, s2));
    printf("%d\n", mystrcspn(a, s2));
    printf("%s\n", strpbrk(a, s2));
    printf("%s\n", mystrpbrk(a, s2));
    printf("%s\n", strstr(a, s2));
    printf("%s\n", mystrstr(a, s2));

    char *token = strtok(aa, s2);
    do
    {
        printf("token: %s\n", token);
    } while (token = strtok(NULL, s2));
    printf("\n");

    token = mystrtok(bb, s2);
    do
    {
        printf("token: %s\n", token);
    } while (token = mystrtok(NULL, s2));

    return 0;
}