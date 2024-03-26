#include "mychain.h"



int32_t chain_rule( sPoly *pResult , const sPoly *pFy, const sPoly *pFx ){

    if(pFy == NULL || pFx == NULL) return -1;
    PQ y[1005] , x[1005] ,  ans[1005];
    int y_size = struct_PQ(y,pFy) , x_size = struct_PQ(x,pFx),ans_size = 0;
    //debug_PQ(x,x_size);
    for(int i=0;i<y_size;i++){
        PQ temp[1005];
        int temp_size = multiply_PQ(temp,x,x_size,y[i].p);
        //debug_PQ(temp,temp_size);
        for(int j=0;j<temp_size;j++) temp[j].c *= y[i].c;
        //debug_PQ(temp,temp_size);
        ans_size = merge_PQ(temp,ans,temp_size,ans_size);
        //debug_PQ(ans,ans_size);
    }
    sort_PQ(ans,ans_size);
    //debug_PQ(ans,ans_size);
    PQ2sPoly(ans_size,ans,pResult);
    //(ans);
    ////free(x);
    //free(y);
    return 0;

}

void debug_PQ(PQ *a, int32_t a_size){
    printf("Debug a.size: %d\nDebug a.p: ",a_size);
    for(int i=0;i<a_size;i++)printf("%d",a[i].p);
    printf("\nDebug a.c: ");
    for(int i=0;i<a_size;i++)printf("%d ",a[i].c);
    printf("\n");
}

void PQ2sPoly(int32_t ans_size , PQ *ans, sPoly *poly_ans){
    
    int qqq = 0;
    for(int i=0;i<ans_size;i++){
        if(!ans[i].c)qqq++;
    }
    ans_size -= qqq;
    poly_ans->size = ans_size;
    poly_ans->pCoefficients = calloc(ans_size,sizeof(int32_t));
    poly_ans->pPowers = calloc(ans_size,sizeof(uint32_t));
    int oldidx = 0,newidx = 0;
    while(newidx < ans_size){
        if(ans[oldidx].c){
            poly_ans->pCoefficients[newidx] = ans[oldidx].c;
            poly_ans->pPowers[newidx] = ans[oldidx].p;
            newidx ++;
        }
        oldidx ++;
    }
}

void sort_PQ(PQ *ans,int32_t ans_size){

    for(int i=0;i<ans_size;i++){
        for(int j=i+1;j<ans_size;j++){
            if(ans[j].p > ans[i].p){
                PQ temp;
                temp = ans[i];
                ans[i] = ans[j];
                ans[j] = temp;
            }
        }
    }

}

int32_t merge_PQ(PQ *a,PQ *b,int32_t a_size,int32_t b_size){
    for(int i=0;i<a_size;i++){
        int transfered = 0;
        for(int j=0;j<b_size;j++){
            if(b[j].p == a[i].p){
                transfered = 1;
                b[j].c += a[i].c;
                break;
            }
        }
        if(!transfered){
            b[b_size] = a[i];
            b_size ++;
        }
    }
    return b_size;
}

int32_t multiply_PQ(PQ *temp,const PQ *x,int32_t size,int32_t times){

    int temp_size = size;
    if(times == 0) return 0;
    for(int i=0;i<size;i++){
        temp[i].c = x[i].c;
        temp[i].p = x[i].p;
    }
    //debug_PQ(temp,temp_size);

    for(int i=0;i<times-1;i++){
        int temp_ = 0;
        PQ temp_PQ[1005];
        for(int j=0;j<temp_size;j++){
            for(int k=0;k<size;k++){
                PQ new[1];
                int transfered = 0;
                new[0].c = temp[j].c * x[k].c;
                new[0].p = temp[j].p + x[k].p;
                temp_ = merge_PQ(new,temp_PQ,1,temp_);
                //debug_PQ(temp_PQ,temp_);
            }
        }
        //debug_PQ(temp_PQ,temp_);
        temp_size = copy_PQ(temp_PQ,temp,temp_);
        //debug_PQ(temp,temp_size);
        //free(temp_PQ);
    }
    //debug_PQ(temp,temp_size);
    temp_size = count_PQ(temp,temp_size);
    //debug_PQ(temp,temp_size);
    return temp_size;

    

}

int32_t count_PQ(PQ *temp,int32_t temp_size){

    PQ temp_PQ[temp_size];
    int temp_ = 0;
    for(int i=0;i<temp_size;i++){
        if(temp[i].c==0)continue;
        if(temp[i].p > 0){
            temp_PQ[temp_].c = temp[i].c * temp[i].p;
            temp_PQ[temp_].p = temp[i].p - 1;
            temp_ ++;
        }
    }
    copy_PQ(temp_PQ,temp,temp_);
    return temp_;

}

int32_t copy_PQ(PQ *a, PQ*b,int32_t a_size){
    for(int i=0;i<a_size;i++)b[i] = a[i];
    return a_size;
}

int32_t struct_PQ(PQ *y, const sPoly *pFy){
    int32_t y_size = 0;
    for(int i=0;i<pFy->size;i++){
        int transfered = 0;
        for(int j=0;j<y_size;j++){
            if(y[j].p == pFy->pPowers[i]){
                transfered = 1;
                y[j].c += pFy->pCoefficients[i];
                break;
            }
        }
        if(!transfered){
            y[y_size].c = pFy->pCoefficients[i];
            y[y_size].p = pFy->pPowers[i];
            y_size ++;
        }
        //debug_PQ(y,y_size);
    }
    return y_size;
}