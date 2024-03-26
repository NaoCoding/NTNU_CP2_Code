#include "mychain.h"

int main(){

    sPoly a,b;
    a.size = 1;
    uint32_t appower[] = {2};
    int32_t apcoe[] = {1};
    a.pPowers = appower;
    a.pCoefficients = apcoe;

    b.size = 3;
    uint32_t bppower[] = {2,1,0};
    int32_t bpcoe[] = {4,-3,1};
    b.pPowers = bppower;
    b.pCoefficients = bpcoe;

    sPoly rec;
    int return_value = chain_rule(&rec,&a,&b);
    printf("Return value : %d\n",return_value);
    printf("Return size : %d\n",rec.size);
    printf("Return Powers : ");
    for(int i=0;i<rec.size;i++)printf("%ld ",rec.pPowers[i]);
    printf("\nReturn pCoefficients : ");
    for(int i=0;i<rec.size;i++)printf("%ld ",rec.pCoefficients[i]);

}