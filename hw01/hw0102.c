#include "mycal.h"



int main(){


    char *Result = NULL;
    char input[10000];
    gets(input);
    int32_t a;
    scanf("%d",&a);
    int32_t return_value = calculate(input,a,&Result);
    printf("%d %s\n",return_value,Result);
    

}
