#include"mymaze.h"

static uint64_t answer = 0xFFFFFFFF;
static uint32_t length = 0;
static sPoint *paths;
static uint32_t r,c;

int32_t been(sPoint *p,uint32_t i,uint32_t j,uint32_t len){
    for(int a=0;a<len-1;a++)if(p[a].row == i && p[a].col == j) return 1;
    return 0;
}

void dfs(struct node room[r][c],uint32_t i ,uint32_t j,sPoint *p,uint32_t cost,uint32_t len){

    sPoint *q = calloc(len,sizeof(sPoint));

    for(int i=0;i<len-1;i++){
        q[i].row = p[i].row;
        q[i].col = p[i].col;
    }
    q[len-1].row = i;
    q[len-1].col = j;
    if(cost > answer)return;
    if(i==r-1 && j == c-1){
        answer = cost;
        paths = q;
        length = len;   
        return;
    }
    // 1: up ,2:right , 3:down,4:left
    
    if(room[i][j].up  && !been(p,i-1,j,len))dfs(room,i-1,j,q,cost+room[i-1][j].val,len+1);
    if(room[i][j].right  && !been(p,i,j+1,len))dfs(room,i,j+1,q,cost+room[i][j+1].val,len+1);
    if(room[i][j].down  && !been(p,i+1,j,len))dfs(room,i+1,j,q,cost+room[i+1][j].val,len+1);
    if(room[i][j].left  && !been(p,i,j-1,len))dfs(room,i,j-1,q,cost+room[i][j-1].val,len+1);

    free(q);
    //free(p);
}
int32_t find_min_path( const sRoom *pMaze , const uint8_t row, const uint8_t col, sPath *pMinPath ){
    if(pMaze == NULL) return -1;
    r = row;
    c = col;
    struct node room[row][col];
    for(int u=0;u<row*col;u++){
        room[u/col][u%col].val = pMaze[u].cost;
        room[u/col][u%col].up = 0;
        room[u/col][u%col].down = 0;
        room[u/col][u%col].left = 0;
        room[u/col][u%col].right = 0;
        room[u/col][u%col].door = count_door(pMaze[u].doors);
    }
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
             if(i != row - 1){
                if(room[i][j].door[2] == room[i+1][j].door[0]){
                    room[i][j].down = 1;
                    room[i+1][j].up = 1;
                } 
             }
             if(j != col - 1){
                if(room[i][j].door[1] == room[i][j+1].door[3]){
                    room[i][j].right = 1;
                    room[i][j+1].left = 1;
                }
             }
        }
    }
    sPoint *root = NULL;
    dfs(room,0,0,root,room[0][0].val,1);
    sPath ans;
    *pMinPath = ans;
    pMinPath->length = length;
    pMinPath->pPath = paths;
    pMinPath->cost = answer; 
    free(root);
    for(int u=0;u<row*col;u++){
        free(room[u/col][u%col].door); 
    }
    if(!length) return 0;
    return 1;
}

uint8_t * count_door(uint8_t val){
    uint8_t *return_ = calloc(8,sizeof(uint8_t));
    return_[0] = val/64;
    val %= 64;
    return_[1] = val/16;
    val %= 16;
    return_[2] = val/4;
    val %= 4;
    return_[3] = val;
    return return_;
}