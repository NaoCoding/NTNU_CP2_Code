#ifndef MYCHAIN_H
#define MYCHAIN_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
typedef struct _sPoly
{
uint32_t size;

uint32_t *pPowers;
int32_t *pCoefficients;
} sPoly;

typedef struct _pq{
    uint32_t p;
    int32_t c;
} PQ;

int32_t struct_PQ(PQ *y, const sPoly *pFy);
int32_t chain_rule( sPoly *pResult , const sPoly *pFy, const sPoly *pFx );
int32_t multiply_PQ(PQ *temp,const PQ *x,int32_t size,int32_t times);
int32_t merge_PQ(PQ *temp,PQ *ans,int32_t temp_size,int32_t ans_size);
void sort_PQ(PQ *ans,int32_t ans_size);
void PQ2sPoly(int32_t ans_size , PQ *ans, sPoly *poly_ans);
void debug_PQ(PQ *a, int32_t a_size);
int32_t copy_PQ(PQ *a, PQ*b,int32_t a_size);
int32_t count_PQ(PQ *temp,int32_t temp_size);

#endif