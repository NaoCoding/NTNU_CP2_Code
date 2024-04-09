#include "AndyLuBMP.h"


void Transfer(BMP * a,BMP * b);

BMP  * bmp;
static double Change_ANGEL = 0;


void GETINPUT();


int main(){

    GETINPUT();

    setupBMP(&bmp[1] , &bmp[0]);
    //showBMPInfo(&bmp[0]);


    bmp[1].info->width = round(bmp[0].info->width + bmp[0].info->height* tan(Change_ANGEL));
    bmp[1].file->size = (4-(bmp[1].info->width* 3%4)+bmp[1].info->width* 3)* bmp[1].info->height  + bmp[1].file->offbit;
    bmp[1].info->BMPsize = bmp[1].file->size - bmp[1].file->offbit;
    //printf("%zu %d %ld",bmp[1]->file->size,bmp[1]->info->size,bmp[1]->info->width);
    //printf("\n%zu %d %ld",bmp[0]->file->size,bmp[0]->info->size,bmp[0]->info->width);


    //showBMPInfo(&bmp[0]);
    //showBMPInfo(&bmp[1]);
    writeInHeader(&bmp[1]);
    Transfer(&bmp[0],&bmp[1]);
    fclose(bmp[0].source);
    fclose(bmp[1].source);
    BMPFree(&bmp[0]);
    BMPFree(&bmp[1]);
    free(bmp);



}

void Transfer(BMP * a,BMP * b){

    //b->info->height = 100;
    for(int i=0;i<b->info->height;i++){
        //int n = 0,ss = 0;
        int lWhite = b->info->direction == 1 ? round((b->info->width - a->info->width)*(i+1)/b->info->height) :
        round((b->info->width - a->info->width)*(b->info->height-i-1)/b->info->height);
        int white = 255;
        for(int j=0;j<lWhite;j++){
            for(int k=0;k<b->info->bitcount/8;k++){
                fwrite(&white,1,1,b->source);
                //n++;
            }

        }
        for(int j=0;j<a->info->width;j++){
                uint8_t readIn[b->info->bitcount/8];
                fread(readIn,b->info->bitcount/8,1,a->source);
                fwrite(readIn,b->info->bitcount/8,1,b->source);
                for(int k=0;k<(4 - (a->info->width*b->info->bitcount/8)%4)%4;k++)fread(readIn,1,1,a->source);
            //n+=b->info->bitcount/8;

            }



        for(int j=0;j<b->info->width - a->info->width - lWhite;j++){

                for(int k=0;k<b->info->bitcount/8;k++){
                fwrite(&white,1,1,b->source);
                //n++;
            }

            }

        for(int k=0;k<(4 - (b->info->width*b->info->bitcount/8)%4)%4;k++){
            fwrite("00",1,1,b->source);
            //n++;
        }





    }
    //printf("%d",b->info->width*3);



}



void GETINPUT(){

    printf("Please input a BMP file: ");
    bmp = calloc(2,sizeof(BMP));
    CallocBMP(&bmp[0]);
    CallocBMP(&bmp[1]);
    fgets(bmp[0].path,1025,stdin);
    if(bmp[0].path[strlen(bmp[0].path)-1]==10)bmp[0].path[strlen(bmp[0].path)-1] = 0;
    if(!( bmp[0].source = fopen(bmp[0].path,"rb"))){
        printf("File Not Exist!\n");
        exit(0);
    }
    if(!READBMP(&bmp[0]) || !dotBMPcheck(bmp[0].path)){
        printf("Read file failed, it is not a BMP file\n");
        exit(0);
    }
    printf("Please input the output BMP file name: ");
    fgets(bmp[1].path,1025,stdin);
    if(bmp[1].path[strlen(bmp[1].path)-1]==10)bmp[1].path[strlen(bmp[1].path)-1] = 0;
    if(!strcmp(bmp[0].path,bmp[1].path)){
        printf("Input file cannot be same as output file!\n");
        exit(0);
    }
    if(!dotBMPcheck(bmp[1].path)){
        printf("Read file failed, it is not a BMP file\n");
        exit(0);
    }
    bmp[1].source = fopen(bmp[1].path,"wb");

    printf("Angle (0-90): ");
    scanf("%lf",&Change_ANGEL);
    if(Change_ANGEL >= 90 || Change_ANGEL < 0){
        printf("Wrong Angle ! (0<=angle<90)\n");
        exit(0);
    }
    Change_ANGEL = Change_ANGEL *3.1415926/180;
    

}
