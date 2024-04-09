#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

void initialize();
void Read_Dict();
int GetWeight();
void GUESS();
uint32_t GET_();
void ValidCheck();
int debug_GETWordIdxInDict(char * target);

typedef struct _DIC{
    char word[6];
    int weight;
    int alive;
}DICT;
static uint32_t ans_idx = 0;
static FILE *dict_file;
static DICT dict[10000];
static int dict_size = 0;
static int w_value[26] = {8200, 1500, 2800, 4300, 12700,
                    2200, 2000, 6100, 7000, 150,
                    770, 4000, 2400, 6700, 7500,
                    1900, 95, 6000, 6300, 9100,
                    2800, 980, 2400, 150, 2000, 74};
static char w_appear[5][5];
static char w_must[5];
static char w_mustnot[5];
static char w_count[26];

int main(){

    initialize();
    Read_Dict();
    GUESS();

}

void ValidCheck(){


    for(int i=0;i<dict_size;i++){
        int pq_count[26] = {0};
        for(int j=0;j<5;j++){
            if(!dict[i].alive)break;

            if(w_must[j] && w_must[j]!=dict[i].word[j]) dict[i].alive = 0;
            else if(w_must[j] && w_must[j]==dict[i].word[j])pq_count[dict[i].word[j]-'A'] --;
            pq_count[dict[i].word[j]-'A'] ++;

            if(w_mustnot[j] && w_mustnot[j]==dict[i].word[j]) dict[i].alive = 0;
            for(int k=0;k<5;k++){
                if(w_appear[j][k] && w_appear[j][k] == dict[i].word[j])dict[i].alive = 0;
            }

        }

        for(int j=0;j<26;j++){
            if(pq_count[j]<w_count[j] && w_count[j]) dict[i].alive = 0;
        }
        for(int j=0;j<5;j++){
            if(w_mustnot[j]){
                if(w_count[w_mustnot[j]-'A'] < pq_count[w_mustnot[j]-'A'])dict[i].alive = 0;
            }
        }



    }


}

int debug_GETWordIdxInDict(char * target){

    for(int i=0;i<dict_size;i++)if(!strcmp(dict[i].word,target)) return i;
    return -1;

}

void GUESS(){

    printf("%-10s","Advice: ");
    ans_idx = GET_();
    if(ans_idx==-1){
        printf("No Advice\n");
        return;
    }
    printf("%s\n",dict[ans_idx].word);
    printf("%-10s","Feedback: ");
    char cmd[1025];
    fgets(cmd,1025,stdin);
    if(cmd[strlen(cmd)-1] == 10)cmd[strlen(cmd)-1] = 0;
    if(strlen(cmd)!=5){
        printf("Wrong input!\n");
        GUESS();
        return;
    }
    int G_count = 0;
    for(int i=0;i<5;i++)G_count += cmd[i] == 'G';
        if(G_count==5){
            printf("Congratulations!!\n");
            return;
        }
    for(int i=0;i<5;i++){
        w_must[i] = 0;
        for(int j=0;j<5;j++){
            w_appear[i][j] = 0;
        }
    }
    for(int i=0;i<26;i++)w_count[i] = 0;

    for(int i=0;i<strlen(cmd);i++){
        if(!isupper(cmd[i]) || (cmd[i] !='B'&&cmd[i] !='Y'&&cmd[i] !='G') ){
            printf("Wrong Input\n");
            GUESS();
        return;
        }
        if(cmd[i]=='B'){
            w_mustnot[i] = dict[ans_idx].word[i];
        }
        else if(cmd[i]=='G'){
            w_must[i] = dict[ans_idx].word[i];
        }
        else if(cmd[i]=='Y'){
            w_count[dict[ans_idx].word[i]-'A'] ++;
            for(int j=0;j<5;j++){
                if(!(i==j)){
                w_appear[i][j] = dict[ans_idx].word[i];
                    }
                }
            }
        }


    ValidCheck();


    GUESS();
}

uint32_t GET_(){
    int32_t idx = -1;
    for(int i=0;i<dict_size;i++){
        if(!dict[i].alive)continue;
        if(idx == -1) idx = i;
        else if(dict[i].weight > dict[idx].weight) idx = i;
    }
    return idx;
}


int GetWeight(){
    int ans = 0;
    for(int i=0;i<5;i++)ans += w_value[dict[dict_size].word[i]-'A'];
    return ans;
}

void Read_Dict(){

    char input[1025];
    while(fgets(input,1025,dict_file)){
        if(strlen(input)<6)continue;
        uint32_t len = 0;
        while(input[len] && isalpha(input[len]))len++;

        if((((input[len]==0 || input[len]==10 || input[len]=='/'))) && len==5){
            input[5] = 0;
            for(int i=0;i<5;i++)if(islower(input[i]))input[i] = input[i] - 'a' + 'A';
            strcpy(dict[dict_size].word,input);
            dict[dict_size].weight = GetWeight();
            dict[dict_size].alive = 1;
            dict_size ++;
        }

    }



}

void initialize(){

    dict_file = fopen("en_US.dic","r");
    if(!dict_file){
        printf("File Not Exist!");
        exit(0);
    }
}