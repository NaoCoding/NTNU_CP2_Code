#ifndef _ANDYLUBMP
#define _ANDYLUBMP

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>




typedef struct _BMPFile{
    uint8_t type[2]; //BM
    uint32_t size;
    uint16_t reverse1;
    uint16_t reverse2;
    uint32_t offbit;
} BMPFile;

typedef struct _BMPInfo{
    uint32_t size;
    int32_t width;
    int32_t height;
    int32_t direction;
    uint16_t planes;
    uint16_t bitcount;
    uint32_t compress;
    uint32_t BMPsize;
    int32_t xpixel;
    int32_t ypixel;
    uint32_t coloruse;
    uint32_t colorImportance;
} BMPInfo;

typedef struct _BMP{
    BMPInfo *info;
    BMPFile *file;
    char path[1025];
    FILE * source;

}BMP;

int READBMP(BMP * target);
void showBMPInfo(BMP * target);
void setupBMP(BMP * target, BMP * from);
void writeInHeader(BMP *target);
void BMPFree(BMP * target);
void CallocBMP(BMP * target);
int dotBMPcheck(char * target);
uint8_t ***BMP2ARR(BMP *target);
int BMPLoad(BMP * target, char * path, char * mode);
void ARR2BMP(BMP * target, uint8_t ***arr);
BMP * iniBMP();


#endif