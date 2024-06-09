#ifndef DATABASE_H
#define DATABASE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "linuxlist.h"

struct linklist{
    struct linklist * next;
};

typedef struct _sLabel
{
char *pStr;
struct list_head list;
} sLabel;

typedef struct _sRecord
{
struct list_head data; // Data for each item in this record
struct list_head list; // List for the next record
} sRecord;

typedef struct _sItem
{
char *pData;
struct list_head next; // Data for each item in this record
} sItem;

int32_t setup_table( const struct list_head *pLabelList );
int32_t add( struct list_head *pRecordList , sRecord *pRecord );
int32_t get_size( struct list_head *pRecordList );
int32_t query( struct list_head *pResultList , struct list_head *pRecordList ,char *pCmd );
#endif