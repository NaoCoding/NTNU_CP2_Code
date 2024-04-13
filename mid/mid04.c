#include "myinclude.h"
#include "AndyLuBMP.h"

static uint8_t rgb[8][3] = {{255,255,255},{0,0,0},{255,102,51},{0,204,0},
{204,204,0},{0,0,204},{204,0,204},{0,204,204}};
uint8_t rgbnow = 0;
static uint8_t c[3];
static uint8_t ***arr;
uint8_t di[8]={-1,-1,0,1,1,1,0,-1};
uint8_t dj[8]={0,1,1,1,0,-1,-1,-1};
int8_t direction = 0;
char cmd[1025];
static uint8_t x,y;

void qqq( char * cmd){
    
    for(int i=0;i<strlen(cmd);i++){
        //printf("%d %d",x,y);
        if(cmd[i]=='F'){
            x += dj[direction];
            y -= di[direction];
            for(int j=0;j<3;j++){
                
                arr[y][x][j] = rgb[rgbnow][j];
            }
        }
        else if(cmd[i]=='R')direction = direction == 7 ? 0 : direction + 1;
        else if(cmd[i]=='C')rgbnow = rgbnow == 7 ? 0 : rgbnow + 1;
        else if(cmd[i]=='['){
            int t = 2;
            int idx = i+1;

        }
    }

}

int main(){

    char in[1025];
    printf("Enter the output filename: ");
    scanf("%s",in);
    BMP *b = iniBMP();
    BMPLoad(b,in,"wb");
    char color[1025];
    
    printf("Enter the background color (R,G,B): ");

    
    scanf("\n(%d,%d,%d)",&c[0],&c[1],&c[2]);
    
    

    printf("Type code here: ");
    
    
    scanf("%s",cmd);

    b->file->type[0] = 'B';
    b->file->type[1] = 'M';
    b->file->size = 256 * 256 * 3 + 54;
    b->file->reverse1 = 0;
    b->file->reverse2 = 0;
    b->file->offbit = 54;

    b->info->size = 40;
    b->info->width = 256;
    b->info->height = 256;
    b->info->planes = 1;
    b->info->bitcount = 24;
    b->info->BMPsize = 256 * 256 * 3;
    b->info->compress = 0;
    b->info->xpixel = 256 * 3;
    b->info->ypixel = 256;
    b->info->colorImportance = 0;
    b->info->coloruse = 0;
    writeInHeader(b);
    x = 127;
    y = 127;
    arr = calloc(256,sizeof(uint8_t **));
    for(int i=0;i<256;i++){
        arr[i] = calloc(256,sizeof(uint8_t *));
        for(int j=0;j<256;j++){
            arr[i][j] = calloc(3,sizeof(uint8_t));
            for(int k=0;k<3;k++){
                arr[i][j][k] = c[k];
            
            }
        }
    }
    qqq(cmd);

    ARR2BMP(b,arr);

    fclose(b->source);
    BMPFree(b);



}