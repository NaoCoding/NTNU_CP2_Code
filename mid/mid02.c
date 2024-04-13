#include "myinclude.h"

char ascii[128][100];

int main(){

    FILE *codebook , *input, *output;
    printf("Codebook: ");
    char in[1025];
    fgets(in,1025,stdin);
    if(in[strlen(in)-1] == 10) in[strlen(in)-1]= 0;
    if(!(codebook=fopen(in,"r"))){
        printf("File not found");
        return 0;
    }
    printf("Input File: ");
    fgets(in,1025,stdin);
    if(in[strlen(in)-1] == 10) in[strlen(in)-1]= 0;
    if(!(input=fopen(in,"r"))){
        printf("File not found");
        return 0;
    }
    printf("Output File: ");
    fgets(in,1025,stdin);
    if(in[strlen(in)-1] == 10) in[strlen(in)-1]= 0;
    output=fopen(in,"wb");

    char line[1025];
    while(fgets(line,1025,codebook)){
        uint8_t idx;
        if(line[0]=='s'&&line[1]=='p') idx = ' ';
        else if(line[0]=='c' && line[1]=='o')idx = ',';
        else if(line[0]=='p'&&line[1]=='e')idx='.';
        else idx = line[0];
        uint8_t rrr = 1;
        while(line[rrr]!=':')rrr++;
        rrr+=2;
        strcpy(ascii[idx],line+rrr);
        if(ascii[idx][strlen(ascii[idx])-1]==10)ascii[idx][strlen(ascii[idx])-1]=0;

    }
    int idx = 0;
    uint8_t m = 0;
    while(fgets(line,1025,input)){
        
        for(int i=0;i<strlen(line);i++){
            //printf("%s",ascii[line[i]]);
            for(int j=0;j<strlen(ascii[line[i]]);j++){
                m = m * 2 + (ascii[line[i]][j]=='1');
                idx ++ ;
                
                if(idx == 8){
                    idx = 0;
                    fwrite(&m,1,1,output);
                    m = 0;
                }
            }
            //printf("%d\n",m);
        }
    }
    m = m * 2 + 1;
    idx += 1;
    while(idx< 8){
        m = m * 2;
        idx ++;
    }
    fwrite(&m,1,1,output);
    //printf("%s",r);
    fclose(codebook);
    fclose(input);
    fclose(output);





}