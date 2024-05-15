#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdint.h>
#include "bmp.h"


BMP *input;
BMP *output;
static int64_t height,width;
static int64_t length = 0;
static uint8_t hset = 0,wset = 0, lset = 0;
static uint8_t iset = 0,oset = 0;

void display_help(){

    printf("./hw0305 [options]\n");
    printf("  -i, --input, mandatory: input file path\n");
    printf("  -o, --output, mandatory: output file path\n");
    printf("  -w, --width, mandatory: output bmp file widths\n");
    printf("  -h, --height, mandatory: output bmp file height\n");
    printf("  -l, --line, mandatory: the radius of line\n");
    printf("  -H, --help, option: show help message\n");
    exit(0);

}

int main(int argc, char*argv[]){
    //printf("?????");
    struct option opts[] = {
        {"input",1,NULL,'i'},
        {"output",1,NULL,'o'},
        {"width",1,NULL,'w'},
        {"height",1,NULL,'h'},
        {"line",1,NULL,'l'},
        {"help",0,NULL,'H'}
    };

    char cas;
    output = iniBMP();
    input = iniBMP();

    while((cas = getopt_long(argc,argv,"i:o:w:h:l:H",opts,NULL)) != -1){

        switch (cas)
        {
        case 'H':
            display_help();
            break;
        case 'h':
            hset = 1;
            height = atoi(optarg);
            break;
        case 'w':
            wset = 1;
            width = atoi(optarg);
            break;
        case 'i':
            iset = 1;
            if(!BMPLoad(input,optarg,"rb")){
                printf("file not found\n");
                exit(0);
            }
            break;
        case 'o':
            oset = 1;
            BMPLoad(output,optarg,"wb");
            break;
        case 'l':
           lset = 1;
           length = atoi(optarg);
            break;
        default:
            break;
        }

    }
    if(optind != argc || iset + oset + lset + wset + hset < 5){
        printf("error\n");
        exit(0);
    }
    if(!width || !height){
        printf("error");
        exit(0);
    }
    //printf("1");
    READBMP(input);
    setupBMP(output,input);
    uint8_t *** arr = BMP2ARR(input);
    uint64_t t[3][256];
    uint64_t highest = 0;
    for(int i=0;i<3;i++){
        for(int j=0;j<256;j++) t[i][j] = 0;
    }

    for(int i=0;i<input->info->height;i++){
        for(int j=0;j<input->info->width;j++){
            for(int k=0;k<3;k++){
                t[k][arr[i][j][k]] += 1;
                //printf("%lu\n",t[k][arr[i][j][k]]);
                highest = highest > t[k][arr[i][j][k]] ? highest : t[k][arr[i][j][k]];
            }
        }
    }
    

    output->info->width = width;
    output->info->height = height;
    output->file->size = ((4-(output->info->width* 3%4))%4+output->info->width* 3)* output->info->height  + output->file->offbit;
    output->info->BMPsize = output->file->size - output->file->offbit; 
    //showBMPInfo(output);

    //printf("%lu",highest);
    free(arr);

    arr = calloc(height,sizeof(uint8_t **));
    for(int i=0;i<height;i++){
        arr[i] = calloc(width,sizeof(uint8_t * ));
        for(int j=0;j<width;j++){
            arr[i][j] = calloc(3,sizeof(uint8_t));
        }
    }
    uint64_t last;
    for(int i=0;i<3;i++){
        for(int j=0;j<256;j++){
            arr[(uint64_t)(((long double)t[i][j] / (long double)highest) * (height - 1))][(uint64_t)(((long double)j/255) * (long double)(width-1)) ][i] = 255;
            if(j){
                long double xs = last;
                long double xe = (((long double)t[i][j] / (long double)highest) * (height - 1));
                long double ys = (((long double)(j)/255) * (long double)(width-1));
                long double ye = (((long double)(j-1)/255) * (long double)(width-1));
                for(long double a = ye;a<=ys;a+=0.01){
                    long double pili = (a-ye)/(ys-ye);
                    //printf("%Lf",pili);
                    uint64_t iii = (uint64_t)(pili * (xe-xs) + xs);
                    arr[iii][(uint64_t)(a)][i] = 255;
                    uint64_t q = length-1;
                    uint64_t col;
                    
                    for(uint64_t b = a-1;(q > 0 && b >= 0 && b < width);b--){
                        col = 255 * (long double)((long double)(q+1)/(long double)(length+1));
                        if(col > arr[iii][b][i]) arr[iii][b][i] = col;
                        q--;
                    }

                    q = length-1;
                    
                    for(uint64_t b = a+1;(q > 0 && b >= 0 && b < width);b++){
                         col = 255 * (long double)((long double)(q+1)/(long double)(length+1));
                        if(col > arr[iii][b][i]) arr[iii][b][i] = col;
                        q--;
                    }
                    q = length-1;
                    for(uint64_t b = iii+1;(q > 0 && b >= 0 && b < height);b++){
                         col = 255 * (long double)((long double)(q+1)/(long double)(length+1));
                        uint64_t p = q - 1;
                        if(col > arr[b][(uint64_t)(a)][i]) arr[b][(uint64_t)(a)][i] = col;
                        
                        for(uint64_t c = a-1;(p>0 && c >=0 && c < width);c--){
                            col = 255 * (long double)((long double)(p+1)/(long double)(length+1));
                            if(col > arr[b][c][i]) arr[b][c][i] = col;
                            p --;
                        }
                        p = q - 1;
                        for(uint64_t c = a+1;(p>0 && c >=0 && c < width);c++){
                            col = 255 * (long double)((long double)(p+1)/(long double)(length+1));
                            if(col > arr[b][c][i]) arr[b][c][i] = col;
                            p --;
                        }
                        q--;
                    }
                    q = length-1;
                    for(uint64_t b = iii-1;(q > 0 && b >= 0 && b < height);b--){
                         col = 255 * (long double)((long double)(q+1)/(long double)(length+1));
                        uint64_t p = q - 1;
                        if(col > arr[b][(uint64_t)(a)][i]) arr[b][(uint64_t)(a)][i] = col;
                        
                        for(uint64_t c = a-1;(p>0 && c >=0 && c < width);c--){
                            col = 255 * (long double)((long double)(p+1)/(long double)(length+1));
                            if(col > arr[b][c][i]) arr[b][c][i] = col;
                            p --;
                        }
                        p = q - 1;
                        for(uint64_t c = a+1;(p>0 && c >=0 && c < width);c++){
                            col = 255 * (long double)((long double)(p+1)/(long double)(length+1));
                            if(col > arr[b][c][i]) arr[b][c][i] = col;
                            p --;
                        }
                        q--;
                    }
                }
                
            }
            last = (uint64_t)(((long double)t[i][j] / (long double)highest) * (height - 1));
        }
    }

    



    writeInHeader(output);
    ARR2BMP(output,arr);
    BMPFree(output);
    fclose(output->source);
}
