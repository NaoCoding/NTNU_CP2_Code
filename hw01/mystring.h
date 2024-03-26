#ifndef MYSTRING_H
#define MYSTRING_H
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

char *mystrchr(const char *s, int c);
char *mystrrchr(const char *s, int c);
size_t mystrspn(const char *s, const char *accept);
size_t mystrcspn(const char *s, const char *reject);
char *mystrpbrk(const char *s, const char *accept);
char *mystrstr(const char *haystack , const char *needle);
char *mystrtok(char *str, const char *delim);
int indelim(char *str, const char *delim);

#endif