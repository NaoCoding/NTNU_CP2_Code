#include "myinclude.h"
#include "AndyLuBMP.h"

int main(){
    BMP * cover = iniBMP();
    BMP * new = iniBMP();
    BMP * output = iniBMP();
    int64_t x,y,w,h;
    char in[1025];
    printf("cover:" );
    fgets(in,1025,stdin);
    if(in[strlen(in)-1] == 10 )in[strlen(in)-1] = 0;
    if(!BMPLoad(cover,in,"rb")){
        printf("file not found");
        return 0;
    }
    printf("x (in pixel): ");
    scanf("%ld",&x);
    printf("y (in pixel): ");
    scanf("%ld",&y);
    printf("w (in pixel): ");
    scanf("%ld",&w);
    printf("h (in pixel): ");
    scanf("%ld",&h);
    printf("new:" );
    fgets(new->path,1025,stdin);
    if(new->path[0] == '\n')fgets(new->path,1025,stdin);
    if(new->path[strlen(new->path)-1] == 10 )new->path[strlen(new->path)-1] = 0;
    if(!BMPLoad(new,new->path,"rb")){
        printf("file not found");
        return 0;
    }
    printf("output:" );
    fgets(output->path,1025,stdin);
    if(output->path[strlen(output->path)-1] == 10 )output->path[strlen(output->path)-1] = 0;
    BMPLoad(output,output->path,"wb");

    READBMP(cover);
    READBMP(new);
    setupBMP(output,cover);
    writeInHeader(output);
    //showBMPInfo(output);
    uint8_t ***arr = BMP2ARR(cover);
    uint8_t ***arr2 = BMP2ARR(new);
    //printf("?");
   
    double ii = new->info->height/h , jj = new->info->width/w;
    double iii = new->info->height-1, jjj = 0;
    //printf("%lf %lf",ii,jj);
    int64_t r = 0,rr = 0;
    for(int i=cover->info->height - 1 - y;i > (cover->info->height - 1 - y - h);i--){
        for(int j=x;j < cover->info->width;j++){
            if(i>=0&&j>=0&&i<cover->info->height && j < cover->info->height){
                for(int k=0;k<3;k++){
                arr[i][j][k] = arr2[(int64_t)iii][(int64_t)jjj][k];
                //printf("%d",arr[i][j][k]);
                
            }
            }
            jjj += jj;
            rr += 1;
            if(rr>=w) break;
            while(jjj>=new->info->width) jjj --;
            //printf("%lf\n",jjj);
        }
        r += 1;
        rr = 0;
        if(r==h)break;
        iii -= ii;
        while(iii < 0) iii++;
        jjj = 0;
    }

    ARR2BMP(output,arr);
    fclose(output->source);
    fclose(cover->source);
    fclose(new->source);
    BMPFree(cover);
    BMPFree(new);
    BMPFree(output);
    free(arr);
    free(arr2);

    
}