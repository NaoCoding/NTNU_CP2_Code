#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdint.h>
#include "bmp.h"

static int mode = 0;
static int bits = 1;
static uint64_t size = 0;
BMP * bmp;
FILE * data;


uint8_t *** ARR_U(uint8_t **** arr){

    uint8_t *** target = calloc(bmp->info->height,sizeof(uint8_t **));
    for(int i=0;i<bmp->info->height;i++){
        target[i] = calloc(bmp->info->width,sizeof(uint8_t *));
        for(int j=0;j<bmp->info->width;j++){
            target[i][j] = calloc(3,sizeof(uint8_t));
            for(int k=0;k<3;k++){
                for(int l=0;l<8;l++){
                    target[i][j][k] = target[i][j][k] * 2 + arr[i][j][k][l];
                }
            }
        }
    }

    return target;

}

uint8_t * toBitArray(uint8_t target){

    uint8_t * re = calloc(8,sizeof(uint8_t));
    uint8_t qq = 128;
    uint8_t i = 0;
    while(qq){
        re[i++] = target/qq;
        target %= qq;
        qq  /= 2;
    }
    return re;

}

uint8_t ArraytoBit(uint8_t * arr){

    uint8_t q = 0;
    for(int i=0;i<8;i++)q = q * 2 + arr[i];
    return q;

}

uint8_t **** U_ARR(uint8_t *** arr){

    uint8_t ****target = calloc(bmp->info->height,sizeof(uint8_t***));

    for(int i=0;i<bmp->info->height;i++){
        target[i] = calloc(bmp->info->width,sizeof(uint8_t **));
        for(int j=0;j<bmp->info->width;j++){
            target[i][j] = calloc(3,sizeof(uint8_t *));
            for(int k=0;k<3;k++){
                target[i][j][k] = toBitArray(arr[i][j][k]);
            }
        }
    }
    //free(arr);
    return target;

}


void getDataSize(){
    fseek(data,0L,SEEK_END);
    size = ftell(data);
    fseek(data,0L,SEEK_SET);

}

int main(int argc, char*argv[]){

    struct option opts[] = {
        {"write",0,NULL,'w'},
        {"extract",0,NULL,'e'},
        {"bits",1,NULL,'b'}
    };

    char cas;
    while((cas = getopt_long(argc,argv,"web:",opts,NULL)) != -1){

        switch (cas)
        {
        case 'w':
            if(mode){
                printf("write and extract cannot appear at the same time\n");
                exit(0);
            }
            mode = 1;
            break;
        case 'e':
            if(mode){
                printf("write and extract cannot appear at the same time\n");
                exit(0);
            }
            mode = 2;
            break;
        
        case 'b':
            bits = atoi(optarg);
            break;
        default:
            printf("Invalid Option!\n");
            exit(0);
            break;
        }

    }

    if(optind != argc - 2){
        printf("cover bmp and secret data amount is invalid\n");
        exit(0);
    }

    if(bits <= 0 ||!mode || bits > 8){
        printf("Bits or mode declare error\n");
        exit(0);
    }

    if(mode==1){
        data = fopen(argv[argc - 1],"rb");
        if(!data){
            printf("file not found.\n");
            exit(0);
        }
        
        bmp = iniBMP();

        if(!BMPLoad(bmp,argv[argc-2],"rb")){
            printf("file not found.\n");
            exit(0);
        }

        READBMP(bmp);
        getDataSize();
        if(bmp->info->height < 0 ) bmp->info->height*= -1;
        if(bmp->info->width < 0 ) bmp->info->width *= -1;

        if(bmp->info->BMPsize * bits * 3 < ((uint64_t)size) * 8){
            printf("data is too big for BMP can save!\n");
            exit(0);
        }

        
        uint8_t **** arr2 = U_ARR(BMP2ARR(bmp));
        fclose(bmp->source);

        BMP * target = iniBMP();
        BMPLoad(target,argv[argc-2],"wb");
        setupBMP(target,bmp);
        target->file->reverse2 = ((uint64_t)size)/65536;
        target->file->reverse1 = ((uint64_t)size)%65536;

        writeInHeader(target);

        
        int i = 0, j = 0, k = 0,l=8-bits;
        

        while(size --){
            //printf("%d",size);
            uint8_t temp;
            fread(&temp,1,1,data);
            uint8_t * as = toBitArray(temp);
           
            uint8_t ready = 0;

            while(1){
                //printf("%d",as[ready]);
                arr2[i][j][k][l++] = as[ready++];
                if(l==8){
                    l = 8-bits;
                    k += 1;
                    if(k == 3){
                        k = 0;
                        j += 1;
                        if(j==bmp->info->width){
                            i += 1;
                            j = 0;
                        }
                    }
                }


                if(ready == 8)break;
            }

            free(as);
        }
        ARR2BMP(target,ARR_U(arr2));


        fclose(target->source);
        fclose(data);

        BMPFree(bmp);
        BMPFree(target);
        free(arr2);

    }

    else if(mode == 2){
        bmp = iniBMP();

        if(!BMPLoad(bmp,argv[argc-2],"rb")){
            printf("file not found.\n");
            exit(0);
        }

        READBMP(bmp);
        if(bmp->info->height < 0 ) bmp->info->height*= -1;
        if(bmp->info->width < 0 ) bmp->info->width *= -1;
        uint8_t **** arr2 = U_ARR(BMP2ARR(bmp));
        data = fopen(argv[argc - 1],"wb");
        int a = 0 , b = 0 , c = 0, d = 8-bits;
    
        for(int i=0;(i<bmp->file->reverse2 * 65536 + bmp->file->reverse1);i++){
            uint8_t temp[8] = {0};
            int idx = 0;
            while(idx < 8){
                temp[idx++] = arr2[a][b][c][d++];
                if(d == 8){
                    d = 8-bits;
                    c += 1;
                    if(c==3){
                        c = 0;
                        b += 1;
                        if(b == bmp->info->width){
                            b = 0;
                            a += 1;
                        }
                    }
                }
            }
            uint8_t r = ArraytoBit(temp);
            fwrite(&r,1,1,data);
        }
        fclose(bmp->source);

        BMPFree(bmp);

        //free(arr);
        free(arr2);
        fclose(data);

    }

    

}