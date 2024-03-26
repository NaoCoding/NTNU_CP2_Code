#include "mycal.h"

char *tentobase(long double number,int base){

    char buffer[200] = {0};
    int idx = 0;
    if(number==0){
        buffer[0] = '0';
        idx += 1;
    }
    else if(number<0){
        buffer[0] = '-';
        idx += 1;
        number *= -1;
    }
    long double cons = 1;
    while(cons < number)cons *=base;
    if(cons > number) cons /= base;
    while(cons>=1){
        int target = (int64_t)(number/cons);
        buffer[idx] = target <= 9 ? '0' + target : target - 10 +'A';
        idx ++;
        number -= target * cons;
        cons /= base;
    }
    sprintf(buffer+idx,"_%d",base);
    char *coco = calloc(0,strlen(buffer));
    for(int i=0;i<strlen(buffer);i++) coco[i] = buffer[i];
    return coco;

}

void debug_read(long double *a,int64_t *b){

    while(*a){
        printf("%ld ",(int64_t)*a);
        a++;
    }
    printf("\n");
    while(*b){
        printf("%ld ",*b);
        b++;
    }
    printf("\n");


}

void getsNum(char *chr,char *idx){

    int p = 0;
    while(*idx!='_'){
        if(!(*idx<='F' && *idx>='A')&&!(*idx>='0'&&*idx<='9')){
            return;
        }
        chr[p] = *idx;
        *idx ++;
        p ++;
    }

}

int32_t getsDigit(char *idx){
    int value = 0;
    while(*idx!=' ' &&*idx){
        if(*idx > '9' || *idx <'0') return -1;
        value = value * 10 + *idx - '0';
        idx ++;
    }
    return value;
}

int64_t basetoTen(char *chr,int32_t base){

    int64_t ans = 0;
    //printf("%ld\n",ans);
    for(int32_t i=0;i<strlen(chr);i++){
        int64_t temper = 0;
        if(chr[i] >='A' && chr[i] <='F') temper = chr[i] - 'A' + 10;
        else if(chr[i] >='0' && chr[i] <='9') temper = chr[i] - '0';
        else return -1;
        if(temper>=base)return -1;
        ans = ans * base + temper;

        //printf("%ld\n",ans);
    }
    //printf("debug fn basetoTen return : %ld\n",ans);
    return ans;
}

int32_t calculate( char *pExpr , int32_t base, char **ppResult ){
    char *idx = pExpr;
    long double *numbers = calloc(100,sizeof(long double));
    int64_t *expression = calloc(100,sizeof(int64_t));
    int32_t p = 0;
    if(pExpr == NULL){free(numbers);
    free(expression);
    //free(nnum);
    //free(nexp);
return -1;}
    while(*idx){
        //printf("debug : %d\n",idx);
        char chr[100] = {0};
        getsNum(chr,idx); //prefix
        idx += strlen(chr);
        //printf("debug getsNum: %s %c\n",chr,*idx);
        if(*idx == '_')idx++; //space
        else{free(numbers);
    free(expression);
    //free(nnum);
    //free(nexp);
return -1;}
        //printf("debug getsNum: %s\n",chr);
        int32_t digit = getsDigit(idx); //base
        if(digit> 16 || digit <2){free(numbers);
    free(expression);
    //free(nnum);
    //free(nexp);
return -1;} //base valid
        //printf("debug digit: %d\n",digit);
        idx += digit > 9 ? 2 : 1;
        int64_t temp_num = basetoTen(chr,digit);
        if(temp_num<0){free(numbers);
    free(expression);
    //free(nnum);
    //free(nexp);
return -1;}
        numbers[p] = temp_num;
        //printf("debug numbers: %ld\n",(int64_t)numbers[p]);

        if(!*idx) break;
        idx ++;
        if(*(idx) != '+' && *(idx) != '-' && *idx != '*') return -1;
        if(*idx == '+')expression[p] = 1;
        else if(*idx == '-') expression[p] = 2;
        else expression[p] = 3;
        //printf("debug expression : %d\n",expression[p]);
        idx += 1;
        if(*idx != ' '){free(numbers);
    free(expression);
    //free(nnum);
    //free(nexp);
return -1;}
        idx ++;
        p ++;

    }
    p += 1;
    //debug_read(numbers,expression);
    int32_t minus_count = 0;
    for(int i=0;expression[i];i++){
        if(expression[i] == 3){
            numbers[i+1] *= numbers[i];
            numbers[i] = -1;
            expression[i] = -1;
            minus_count ++;
        }
    }
    //for(int i=0;i<p;i++)printf("%ld ",(int64_t)numbers[i]);
    long double *nnum = calloc(100,sizeof(long double));
    int64_t *nexp = calloc(100,sizeof(int64_t));
    //printf("\n%d",p);
    int nnumq = p , nnumidx = 0, nnumptr = 0;
    while(minus_count){
        if(numbers[nnumidx] == -1) minus_count --;
        else{
            nnum[nnumptr] = numbers[nnumidx];
            nnumptr ++;
        }
        nnumidx ++;
    }
    while(nnumidx<nnumq){
        nnum[nnumptr] = numbers[nnumidx];
        nnumptr ++;
        nnumidx ++;
    }
    //printf("debug recount num success %d\n",p);
    nnumq = p-1 , nnumidx = 0, nnumptr = 0;
    while(nnumq){
        if(expression[nnumidx] <= 0) nnumq --;
        else{
            nexp[nnumptr] = expression[nnumidx];
            nnumptr ++;
        }
        nnumidx ++;
    }
    //printf("debug re_counting: success\n");
    //debug_read(nnum,nexp);
    //for(int i=0;i<p;i++)printf("%ld ",(int64_t)nnum[i]);
    p = 0;
    for(int i=0;nexp[i];i++){
        if(nexp[i] == 1)nnum[i+1] += nnum[i];
        else nnum[i+1] = nnum[i] - nnum[i+1];
        p ++;
        //printf("%ld %d\n",(int64_t)nnum[i+1],i);
    }
    //printf("%ld %ld\n",(int64_t)nnum[p],base);
    *ppResult = tentobase(nnum[p],base);

    free(numbers);
    free(expression);
    free(nnum);
    free(nexp);
    return 0;
}