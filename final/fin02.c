#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "bmp.h"

void display_help();

#define PI (3.14159265358979323846)
#define PI_DIV_180 (0.017453292519943296)
#define DegToRad(x)	((x)*PI_DIV_180)


char * url;
int32_t score[8] = {0};

int main(int argc, char * argv[]){


    struct option optOption[] = 
    {
        {"help",0,NULL,'h'},
    };
    
    int32_t optin;
    while((optin = getopt_long(argc,argv,"h",optOption,NULL)) != -1){

        switch(optin){
            case 'h':
                display_help();
                break;
            default:
                display_help();
                break;
        }
    }

    
    url = calloc(1025,sizeof(char));
    strcpy(url, "https://cpe.cse.nsysu.edu.tw/cpe/scoreboard/");
    sprintf(url + strlen(url) , "%s",argv[optind]);

    CURL *wadwad;
    CURLcode qq;
    wadwad = curl_easy_init();
    if ( wadwad )
    {
        //printf("hi\n");
        curl_easy_setopt( wadwad, CURLOPT_URL,url );
        FILE *result = fopen("result.html","w");
        assert(result);
        curl_easy_setopt( wadwad, CURLOPT_WRITEDATA, result );
        qq = curl_easy_perform(wadwad);
        if (qq != CURLE_OK) 
        {
            
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(qq));
        }
        fclose(result);
     
       curl_easy_cleanup(wadwad);
    }

    FILE * html = fopen("result.html","r");
    int32_t didResultList = 0;
    char in[1025];
    int index = 0;
    

    while(fgets(in,1025,html)){
        if(in[0] == 'v'){
            if(in[1] == 'a'){
                if(in[2] == 'r'){
                    if(in[3] == ' '){
                        if(in[4] == 'p'){
                            if(in[5] == 'r'){
                                if(in[6] == 'o'){
                                    if(in[7] == 'b'){
                                        if(in[8] == 'l'){
                                            if(in[9] == 'e'){
                                                if(in[10] == 'm'){
                                                    if(in[11] == 'L'){
                                                        if(in[12] == 'i'){
                                                            if(in[13] == 's'){
                                                                didResultList = 1;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    while(fread(in+index,1,1,html) && didResultList){

        if(in[index] == 'E'){
            if(in[index - 1 >= 0 ? index - 1 : index - 1 + 5] == 'R'){
                if(in[index - 2 >= 0 ? index - 2 : index - 2 + 5] == 'O'){
                    if(in[index - 3 >= 0 ? index - 3 : index - 3 + 5] == 'C'){
                        if(in[index - 4 >= 0 ? index - 4 : index - 4 + 5] == 'S'){
                            char p[4];
                            fread(p,1,4,html);
                            score[p[3] - '0'] += 1;
                        }
                    }
                }
            }
        }

        index += 1;
        if(index == 5) index = 0;
    }
    fclose(html);

    if(!didResultList || score[0] == 0){
        printf("date not found!\n");
        exit(0);
    }

    long double percentage[8];
    long double qr = 0;
    for(int i=0;i<8;i++) qr += score[i];

    for(int i=0;i<8;i++){
        percentage[i] = ((long double)(score[i]))/qr;
    }

    for(int i=0;i<8;i++){
        printf("%d: ",i);
        score[i] /= 2;
        printf("%d (%.2LF%%)\n",score[i],percentage[i]*100);
    }

    BMP * output = iniBMP();
    BMPLoad(output,"output.bmp","wb");

    output->file->type[0] = 'B';
    output->file->type[1] = 'M';
    output->file->size = 600 * 600 * 3 + 54;
    output->file->reverse1 = 0;
    output->file->reverse2 = 0;
    output->file->offbit = 54;

    output->info->size = 40;
    output->info->width = 600;
    output->info->height = 600;
    output->info->planes = 1;
    output->info->bitcount = 24;
    output->info->BMPsize = 600 * 600 * 3;
    output->info->compress = 0;
    output->info->xpixel = 600 * 3;
    output->info->ypixel = 600;
    output->info->colorImportance = 0;
    output->info->coloruse = 0;

    writeInHeader(output);
    

    uint8_t *** arr = calloc(600,sizeof(uint8_t ** ));
    for(int i=0;i<600;i++){
        arr[i] = calloc(600,sizeof(uint8_t *));
        for(int j=0;j<600;j++){
            arr[i][j] = calloc(3,sizeof(uint8_t));
            for(int k=0;k<3;k++) arr[i][j][k] = 255;
        }
    }
    long double now_deg = 0;
    uint8_t color[8][3] = {
        {255,0,0},{0,255,0},{0,0,255},{125,0,125},{0,255,255},{125,125,0},{0,0,0},{60,60,60}
    };
    for(int a=0;a<8;a++){
        for(long double q = now_deg;q<now_deg + 360 * percentage[a];q+=0.01){
            for(int i=0;i<300;i++){
                for(int k=0;k<3;k++) arr[(int32_t)(599.0 - (299.0 - (long double)(i) * cos(DegToRad(q))))][(int32_t)(300.0 - (long double)(i) * sin(DegToRad(q)))][k] =  color[a][k];
            }
        }
        now_deg += 360 * percentage[a];
    }

    ARR2BMP(output,arr);
    BMPFree(output);
    free(arr);
    system("rm result.html");

}

void display_help(){

    printf("./fin02 --help\n");
    printf("    Usage: fin02 date\n");
    printf("    -h, --help Display this information and exit.\n");
    exit(0);

}