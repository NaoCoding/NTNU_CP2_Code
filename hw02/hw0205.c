#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>


static char prch[16][10] = {"F32","F16","Q4_0","Q4_1"
                        ,"Q4_2","Q4_3","Q5_0","Q5_1"
                        ,"Q8_0","Q8_1","Q2_K","Q3_K"
                        ,"Q4_K","Q5_K","Q6_K","Q8_K"};

typedef struct _header
{
    char magic[4];
    uint32_t version;
    uint64_t tensor;
    uint64_t meta;
    

}H;

typedef struct _metainfo
{
    uint64_t keylen;
    char *key;
    uint32_t type;
    uint64_t valuelen;
    double val;
    float fval;
    
    uint8_t *value;
     //uint64_t qqqq;
     uint8_t arrtype;
    char **arr;
    uint64_t arrl;
    uint8_t arridx;
    uint64_t narr[5];
    uint8_t narridx;
    float farr[5];
    uint8_t fidx;
    double darr[5];
    uint8_t darridx;
    uint64_t u64;
    int64_t i64;

}metainfo;

typedef struct _wiojio{
    char *key;
    uint64_t *shape;
    uint32_t shapesize;
    uint8_t preci;
    uint64_t l;
} T;


int main(){
    double para = 0;
    FILE * file = fopen("model.gguf","rb");
    if(!file){
        printf("GGUF: false\n");
        exit(0);
    }
    H header;
    T *tensor;

    fread(header.magic,4,1,file);
    if(strcmp(header.magic,"GGUF")){
        printf("GGUF: false\n");
        exit(0);
    }
    printf("GGUF: true\n");

    fread(&header.version,4,1,file);
    fread(&header.tensor,8,1,file);
    fread(&header.meta,8,1,file);
    metainfo *M = calloc(header.meta,sizeof(metainfo));
    tensor = calloc(header.tensor,sizeof(T));
    //printf("%d",header.meta);
    for(int i=0;i<header.meta;i++){
        fread(&(M[i].keylen),8,1,file);
        //printf("%d\n",M[i].keylen);
        M[i].key = calloc(M[i].keylen+1,sizeof(char));
        fread(M[i].key,M[i].keylen,1,file);
        fread(&(M[i].type),4,1,file);
        if(M[i].type==9){
            uint64_t t;
            uint32_t cate;
            M[i].arrl = 0;
            M[i].narridx = 0;
            fread(&cate,4,1,file);
            fread(&t,8,1,file);
            M[i].arrtype = cate;
            int idx = 0;
            //printf("%ld %d",t,cate);
            M[i].arr = calloc(5,sizeof(char *));
            for(int j=0;j<t;j++){
                
                if(cate==8){
                uint64_t l;
               
                fread(&l,8,1,file);
                if((M[i].arrl + l > 25 && M[i].arrl) || j > 5){
                    char temp[l];
                    fread(temp,l,1,file);
                }
                else{
                    M[i].arr[j] = calloc(l,sizeof(char));
                    fread(M[i].arr[j],l,1,file);
                    M[i].arrl += l;
                    M[i].arridx = j;
                    //printf("%s",M[i].arr[j]);
                }
                
                }
                else if(cate == 6){
                    if(j>=5){
                        float wedafw;
                        fread(&wedafw,4,1,file);
                    }
                    else{
                        fread(&M[i].farr[j],4,1,file);
                        M[i].fidx = j;
                    }
                }
                else if(cate==7 || cate == 0 || cate == 1){
                    if(j>=5){
                        float wedafw;
                        fread(&wedafw,1,1,file);
                    }
                    else{
                        fread(&M[i].narr[j],1,1,file);
                        M[i].narridx = j;
                    }
                }
                else{
                    if(j>=5){
                        uint32_t wedafw;
                        fread(&wedafw,4,1,file);
                    }
                    else{
                        fread(&M[i].narr[j],4,1,file);
                        M[i].narridx = j;
                    }
                }
                

            }
        }
        else if(M[i].type==6){
            fread(&M[i].fval,4,1,file);

        }
        else if(M[i].type==7 || M[i].type==1 || M[i].type==0){
            fread(&M[i].i64,1,1,file);

        }
        else if(M[i].type==2 || M[i].type==3){
            fread(&M[i].i64,2,1,file);

        }
        else if(M[i].type==12){
            fread(&M[i].val,8,1,file);
        }
        else if(M[i].type==10){
            fread(&M[i].u64,8,1,file);
        }
        else if(M[i].type==11){
            fread(&M[i].i64,8,1,file);
        }
        else if(M[i].type==8){
            fread(&(M[i].valuelen),8,1,file);
            M[i].value = calloc(M[i].valuelen,sizeof(char));
            fread(M[i].value,M[i].valuelen,1,file);
        }
        else{
            M[i].value = calloc(4,sizeof(char));
            fread(M[i].value,4,1,file);
            M[i].i64 = 0;
            for(int j=3;j>=0;j--)M[i].i64 = M[i].i64 * 256 + M[i].value[j];
        }
        
        //printf("%-20s %-20s\n",M[i].key,M[i].value);
    }
    //printf("???");

    for(int i=0;i<header.tensor;i++){
        //printf("???");
        //break;
        fread(&tensor[i].l,8,1,file);
        tensor[i].key = calloc(tensor[i].l+1,sizeof(char));
        fread((tensor[i].key),tensor[i].l,1,file);
        fread(&(tensor[i].shapesize),4,1,file);
        tensor[i].shape = calloc(tensor[i].shapesize,sizeof(uint64_t));
        uint64_t qr = 1;
        for(int j=0;j<tensor[i].shapesize;j++){
            fread(&tensor[i].shape[j],8,1,file);
            qr *= tensor[i].shape[j];
        }
        fread(&tensor[i].preci,4,1,file);
        uint64_t off;
        fread(&off,8,1,file);
        para += qr;
    }


    printf("Parameters: %.0lf\n",para);
    printf("\n%-40s %-40s\n","Metadata","Value");
    printf("%-40s %-40d\n","version",header.version);
    printf("%-40s %-40ld\n","tensor_count",header.tensor);
    printf("%-40s %-40ld\n","meta_count",header.meta);
    for(int i=0;i<header.meta;i++){
        //printf("%d",M[i].keylen);
        if(M[i].type == 9){
            printf("%-40s [",M[i].key);
            if(M[i].arrtype == 8) for(int j=0;j<M[i].arridx;j++)printf("%s, ",M[i].arr[j]);
            else if(M[i].arrtype == 6) for(int j=0;j<M[i].fidx;j++)printf("%.3f, ",M[i].farr[j]);
            else for(int j=0;j<M[i].narridx;j++)printf("%ld, ",M[i].narr[j]);
            printf("...]\n");
        }
        else if(M[i].type==8){
            if(strlen(M[i].value) < 40)printf("%-40s %-40s\n",M[i].key,M[i].value);
            else{
                printf("%-40s ",M[i].key);
                for(int j=0;j<40;j++)printf("%c",M[i].value[j]);
                printf("\n");
            }
        }
        else if(M[i].type==6)printf("%-40s %-40.3lf\n",M[i].key,M[i].fval);
        else if(M[i].type==12)printf("%-40s %-40.3lf\n",M[i].key,M[i].val);
        else if(M[i].type==10)printf("%-40s %-40.3lu\n",M[i].key,M[i].u64);
        else printf("%-40s %-40.0ld\n",M[i].key,M[i].i64);
    }
    printf("\n%-40s %-25s %-10s\n","Tensors","Shape","Precision");
    for(int i=0;i<header.tensor;i++){
        for(int j=i+1;j<header.tensor;j++){
             if(strcmp(tensor[i].key,tensor[j].key)>0){
                T temp;
                temp = tensor[i];
                tensor[i] = tensor[j];
                tensor[j] = temp;
            }
        }
        //printf("%ld",tensor[i].l);
        int same = 0;
        int qq = 0;
        if(i){
            
            while(tensor[i].key[same]==tensor[i-1].key[same]){
                if(tensor[i].key[same]=='.')qq++;
                same++;
            }
            while(qq>=1 && same > 0){
                if(tensor[i].key[same]=='.')qq--;
                same--;
            }
            if(same)same+=2;
            int rest = same;
            while(tensor[i].key[same]==tensor[i-1].key[same] && tensor[i].key[same]!='.')same++;
            if(tensor[i].key[same]!='.')same = rest;
            if(same)same++;
            if(same && !isdigit(tensor[i].key[same-2]))same = 0;
            //same = same > 0 ? same + 2 : 0;

            for(int j=0;j<same;j++)printf(" ");
        }
        for(int j=same;j<tensor[i].l;j++){
            printf("%c",tensor[i].key[j]);
        }
        for(int j=tensor[i].l;j<40;j++)printf(" ");
        char middle[100] = {0};
        middle[0] = '[';
        int asdf = 1;
        
        for(int j=0;j<tensor[i].shapesize;j++){
            sprintf(middle+asdf,"%ld",tensor[i].shape[j]);
            while(tensor[i].shape[j]){
                asdf += 1;
                tensor[i].shape[j] /= 10;
            }
            if(j<tensor[i].shapesize-1){
                middle[asdf++] = ',';
            middle[asdf++] = ' ';
            }
        }
        
        middle[asdf] = ']';
        printf("%-25s",middle);
        printf(" %-10s\n",prch[tensor[i].preci]);
    }
    for(int i=0;i<header.meta;i++){
        free(M[i].key);
        free(M[i].value);
        for(int j=0;j<M[i].arridx;j++)free(M[i].arr[j]);
        free(M[i].arr);
    }
    for(int i=0;i<header.tensor;i++){
        free(tensor[i].shape);
        free(tensor[i].key);
    }
    free(tensor);
    free(M);
    fclose(file);

}
