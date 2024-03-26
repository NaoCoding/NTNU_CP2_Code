#include "mymaze.h"
int main()
{
    int row;
    int col;
    scanf("%hhu", &row);
    scanf("%hhu", &col);
    sRoom inp[row][col];
    //printf("%d %d\n",row,col);
    for (uint8_t i = 0; i < row; i++)
    {
        for (uint8_t j = 0; j < col; j++)
        {
            scanf("%u", &inp[i][j].cost);
            // input doors in four sides
            int up;
            int right;
            int down;
            int left;
            scanf("%hhu %hhu %hhu %hhu", &up, &right, &down, &left);
            inp[i][j].doors = (up << 6) | (right << 4) | (down << 2) | left;
            //rintf("%d ",inp[i][j].doors);
        }
    }
    sPath result;
    result.pPath = NULL;
    result.length = 0;
    result.cost = 0;
    printf("return: %d\n",find_min_path(&(inp[0][0]),row,col,&result));
    printf("cost:%d length:%d\n\n",result.cost,result.length);
    for(uint32_t i=0;i<result.length;i++)
    {
        printf("%d %d\n",result.pPath[i].row,result.pPath[i].col);
    }
    free(result.pPath);
}