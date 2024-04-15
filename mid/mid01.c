#include "myinclude.h"
#include "AndyLuBMP.h"
//remember to free!
static int width = 0;
static int height = 0;
static int p = 0;
static int q = 0;

int checkValid(char * in){

    int left = 0;
    int tempwidth = 0;
    int tt = 0;
    if(strlen(in)<2) return 0;
    for(int i=0;i<strlen(in);i++){
        if(left == 0  && in[i] != '[') return 0;
        if((!i || i == 1) && in[i]!='[') return 0;
        if(in[i]=='[') left += 1;
        if(in[i]==']'){
            left -= 1;
            if(left < 0) return 0;
        }
        if(!isdigit(in[i]) && in[i]!=',' && in[i] !='[' && in[i] !=']')return 0;
        if(in[i]==',' && in[i-1] == '[') return 0;

    }
    if(left) return 0;

    return 1;

}

int64_t **str2arr(char * in){
    p = -1;
    int id = 0,io = 0;
    for(int i=0;i<strlen(in);i++){
        if(in[i]==']'){
            p ++;
            if(!io) io = i;
        }
        else if(in[i] == '['){
            if(!io) id = i;
        }
    }
    q = 1;
    for(int i=id;i<io;i++){
        if(in[i]==',')q++;
    }
    int64_t ** arr = calloc(p,sizeof(int64_t *));
    int64_t idx = 2;
    int64_t now = 0;
    //printf("%d %d\n",p,q);
    for(int i=0;i<p;i++){
        arr[i] = calloc(q,sizeof(int64_t));
        for(int j=0;j<q;j++){
            //printf("%c\n",in[idx]);
            int r  =1;
            if(in[idx] == '-'){
                idx ++;
                r = -1;
            }
            while(isdigit(in[idx])){
                now = now * 10 + in[idx] - '0';
                idx += 1;
            }
            arr[i][j] = now * r;
            now = 0;
            idx += 1;
        }
        idx +=2;
    }
    //printf("?");
    /*
    for(int i=0;i<p;i++){
        for(int j=0;j<q;j++){
            printf("%d ",arr[i][j]);
        }
        printf("\n");
    }*/

    return arr;

}

int64_t **calc(int64_t ** val , int64_t**in){
    if(!val){
        width = q;
        height = p;
        int64_t **n = calloc(height,sizeof(int64_t *));
        for(int i=0;i<p;i++){
            n[i] = calloc(width,sizeof(int64_t));
            for(int j=0;j<q;j++)n[i][j] = in[i][j];
        }
        free(in);
        return n;
    }

    int64_t **n = calloc(height,sizeof(int64_t *));
    //printf("done");make

    for(int64_t i=0;i<height;i++){
        n[i] = calloc(q,sizeof(int64_t));
        for(int64_t j=0;j<q;j++){
            n[i][j] = 0;
            for(int k=0;k<width;k++){
                n[i][j] += val[i][k] * in[k][j]; 
            }
        }
    }
    width = q;
    free(val);
    free(in);
    return n;

}


int main(){

    int64_t **val = NULL;
    int p = 1;
    while(1){
        printf("%d matrix: ",p++);
        char in[1025];
        fgets(in,1025,stdin);
        in[strlen(in)-1] = in[strlen(in)-1] == '\n' ? 0 : in[strlen(in)-1];
        if(!strcmp("end",in))break;
        int r = checkValid(in);
        
        int64_t ** inarr = str2arr(in);
       

        val = calc(val,inarr);
        
        
        
    }
    printf("Result: [");
    for(int i=0;i<height;i++){
        if(i)printf(",");
        printf("[");
        for(int j=0;j<width;j++){
            if(j)printf(",");
            printf("%d",val[i][j]);
        }
        printf("]");
    }
    printf("]");

}
