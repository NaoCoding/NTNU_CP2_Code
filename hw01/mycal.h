#ifndef MYCAL_H
#define MYCAL_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int32_t calculate( char *pExpr , int32_t base, char **ppResult );
int32_t getsDigit(char *idx);
int64_t basetoTen(char *chr , int base);
void getsNum(char *chr,char *idx);
void debug_read(long double *a, int64_t *b);
char *tentobase(long double number,int base);

#endif