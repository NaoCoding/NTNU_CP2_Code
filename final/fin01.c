#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bmp.h"
#include <ctype.h>

void display_help();
char * mergeStr();
char * FC();

char * prefix;
int32_t re[2];

BMP * input, *output;
int32_t file_count = 0;


int main(int argc, char * argv[]){

    prefix = calloc(1025,sizeof(char));
    strcpy(prefix,"output");
    re[0] = 1024;
    re[1] = 768;


    struct option optOption[] = 
    {
        {"help",0,NULL,'h'},
        {"prefix",1,NULL,'p'},
        {"resolution",1,NULL,'r'}
    };

    int32_t optin,index,didX;
    while((optin = getopt_long(argc,argv,"hp:r:",optOption,NULL)) != -1){

        switch(optin){
            
            case 'h':
                display_help();
                break;
            case 'p':

                strcpy(prefix,optarg);
                

                break;
            case 'r':
                didX = 0;
                index = 0;
                re[0] = 0;
                re[1] = 0;

                while(index < strlen(optarg)){
                    if(optarg[index] == 'x'){
                        didX += 1;
                        if(didX > 1){
                            printf("resolution argument error.\n");
                            exit(0);
                        }
                    }
                    else{
                        if(!isdigit(optarg[index])){
                            printf("resolution argument error.\n");
                            exit(0);
                        }
                        re[didX] = re[didX] * 10 + optarg[index] - '0';
                    }
                    index ++;
                }
                break;

            default:
                display_help();
                break;
        }


    }

    if(optind == argc)display_help();
    sprintf(prefix + strlen(prefix),"%s","00");
    
    input = iniBMP();
    if(!BMPLoad(input, argv[optind], "rb")){
        printf("file load error.\n");
        exit(0);
    }
    if(strcmp(argv[optind]+strlen(argv[optind])-5,".mbmp")){
        printf("not .mbmp!\n");
        exit(0);
    }

    while(!feof(input->source)){
        free(input->file);
        free(input->info);
        CallocBMP(input);
        if(!READBMP(input)) break;
        
        uint8_t *** arr = BMP2ARR(input);
        output = iniBMP();
        BMPLoad(output,mergeStr(prefix,FC()),"wb");
        setupBMP(output,input);

        output->info->height = re[1];
        output->info->width = re[0];
        output->file->size = (4-(output->info->width* 3%4)+output->info->width* 3)* output->info->height  + output->file->offbit;
        output->info->BMPsize = output->file->size - output->file->offbit;
        writeInHeader(output);

        uint8_t *** arr2 = calloc(re[1],sizeof(uint8_t **));
        long double py = ((long double)(input->info->height)) / ((long double)(re[1]));
        long double px = ((long double)(input->info->width)) / ((long double)(re[0]));

        for(uint32_t i=0;i<re[1];i++){
            arr2[i] = calloc(re[0],sizeof(uint8_t *));
            for(uint32_t j=0;j<re[0];j++){
                arr2[i][j] = calloc(3,sizeof(uint8_t));
                for(int k=0;k<3;k++){
                    arr2[i][j][k] = arr[(uint32_t)(i*py)][(uint32_t)(j*px)][k];
                }
            }
        }

        ARR2BMP(output,arr2);
        BMPFree(output);
        free(arr);
        free(arr2);
        file_count ++;
    }

    BMPFree(input);
    free(prefix);


}

char * mergeStr(char * a , char * b){

    char * c = calloc(strlen(a) + strlen(b) + 1,sizeof(char));
    for(int i=0;i<strlen(a);i++){
        c[i] = a[i];
    }
    for(int j=strlen(a);j<strlen(a) + strlen(b);j++){
        c[j] = b[j - strlen(a)];
    }


    free(b);
    return c;
}

char * FC(){

    char * n = calloc(1025,sizeof(char));
    sprintf(n,"%d",file_count);
    sprintf(n + strlen(n),"%s",".bmp");
    return n;

}

void display_help(){

    printf("Usage: fin01 [options] file\n");
    printf("    -r, --resolution=widthxheight Setup the resolution. Default: 1024x768.\n");
    printf("    -p, --prefix=str Setup the file name prefix. Default: output.\n");
    printf("    -h, --help Display this information and exit.\n");
    exit(0);

}