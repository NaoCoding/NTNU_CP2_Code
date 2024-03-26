#ifndef MYMAZEHH
#define MYMAZEHH
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static uint32_t r,c;
struct node{
    int left;
    int right;
    int down;
    int up;
    uint8_t *door;
    uint32_t val;
    
};

typedef struct _sRoom{

    uint32_t cost;
    uint8_t doors;

} sRoom;

typedef struct _sPoint
{
uint32_t row;
uint32_t col;
} sPoint;

typedef struct _sPath
{
uint32_t length; // Path length.
uint32_t cost; // Cost
sPoint *pPath; // An array of all points in order.
} sPath;

int32_t find_min_path( const sRoom *pMaze , const uint8_t row, const uint8_t col, sPath *pMinPath );
void dfs(struct node room[r][c],uint32_t i ,uint32_t j,sPoint *p,uint32_t cost,uint32_t len);
uint8_t * count_door(uint8_t val);
int32_t been(sPoint *p,uint32_t i,uint32_t j,uint32_t len);
#endif