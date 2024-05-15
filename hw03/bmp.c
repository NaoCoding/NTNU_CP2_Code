#include "bmp.h"

BMP * iniBMP(){
    BMP * target = calloc(1,sizeof(BMP));
    CallocBMP(target);
    return target;
}

int BMPLoad(BMP * target, char * path, char * mode){
    strcpy(target->path,path);
    target->source = fopen(target->path,mode);
    if(!target->source)return 0;
    return 1;
}

void ARR2BMP(BMP * target, uint8_t ***arr){
    for(int i=0;i<target->info->height;i++){
        for(int j=0;j<target->info->width;j++){
            fwrite(arr[i][j],3,1,target->source);
        }
        for(int j=0;j<(4-target->info->width*3%4)%4;j++)fwrite("00",1,1,target->source);
    }
}

uint8_t ***BMP2ARR(BMP *target){
    uint8_t ***arr = calloc(target->info->height,sizeof(uint8_t**));
    for(int i=0;i<target->info->height;i++){
        arr[i] = calloc(target->info->width,sizeof(uint8_t *));
        for(int j=0;j<target->info->width;j++){
            arr[i][j] = calloc(3,sizeof(uint8_t));
            fread(arr[i][j],1,3,target->source);
        }
        uint8_t temp;
        for(int j=0;j<(4-target->info->width*3%4)%4;j++)fread(&temp,1,1,target->source);
    }
    return arr;
    
}

int dotBMPcheck(char * target){
    int q = strlen(target);
    if(strcmp(target+q-3,"bmp")){
        if(strcmp(target+q-3,"BMP"))return 0;
    }
    return 1;
   
} 

void CallocBMP(BMP * target){
    target->file = calloc(1,sizeof(BMPFile));
    target->info = calloc(1,sizeof(BMPInfo));
}

void BMPFree(BMP * target){
    free(target->file);
    free(target->info);
    free(target);
    //free(target->path);
    //free(target->source);
}

void writeInHeader(BMP * target){

    fwrite(target->file->type,sizeof(target->file->type),1,target->source);
    fwrite(&(target->file->size),sizeof(target->file->size),1,target->source);
    fwrite(&(target->file->reverse1),sizeof(target->file->reverse1),1,target->source);
    fwrite(&(target->file->reverse2),sizeof(target->file->reverse2),1,target->source);
    fwrite(&(target->file->offbit),sizeof(target->file->offbit),1,target->source);

    fwrite(&(target->info->size),sizeof(target->info->size),1,target->source);
    fwrite(&(target->info->width),sizeof(target->info->width),1,target->source);
    fwrite(&(target->info->height),sizeof(target->info->height),1,target->source);
    fwrite(&(target->info->planes),sizeof(target->info->planes),1,target->source);
    fwrite(&(target->info->bitcount),sizeof(target->info->bitcount),1,target->source);
    fwrite(&(target->info->compress),sizeof(target->info->compress),1,target->source);

    fwrite(&(target->info->BMPsize),sizeof(target->info->BMPsize),1,target->source);
    fwrite(&(target->info->xpixel),sizeof(target->info->xpixel),1,target->source);
    fwrite(&(target->info->ypixel),sizeof(target->info->ypixel),1,target->source);
    fwrite(&(target->info->coloruse),sizeof(target->info->coloruse),1,target->source);
    fwrite(&(target->info->colorImportance),sizeof(target->info->colorImportance),1,target->source);

}

void setupBMP(BMP * target , BMP * from){
    
    target->file->type[0] = 'B';
    target->file->type[1] = 'M';
    
    target->file->size = from->info->width * from->info->height * 3 + from->file->offbit;
    target->file->reverse1 = from->file->reverse1;
    target->file->reverse2 = from->file->reverse1;
    target->file->offbit = from->file->offbit;

    target->info->size =  from->info->size;
    target->info->width = from->info->width;
    target->info->height = from->info->height;
    target->info->direction = from->info->direction;
    target->info->planes = from->info->planes;
    target->info->bitcount = from->info->bitcount;
    target->info->compress = from->info->compress;
    target->info->BMPsize = from->info->width * from->info->height * from->info->bitcount/8;
    target->info->xpixel = from->info->xpixel;
    target->info->ypixel = from->info->ypixel;
    target->info->width = from->info->width;
    target->info->coloruse = from->info->coloruse;
    target->info->colorImportance = from->info->colorImportance;

}

int READBMP(BMP * target){
    
    
    fread(target->file->type,2,1,target->source);
    //printf("%s 1",target->file->type);
    if(strcmp(target->file->type,"BM")){
        if(strcmp(target->file->type,"BA")){
            if(strcmp(target->file->type,"CI")){
                if(strcmp(target->file->type,"CP")){
                    if(strcmp(target->file->type,"IC")){
                        if(strcmp(target->file->type,"PT")) return 0;
                    }
                }
            }
        }
    }
    
    fread(&(target->file->size),4,1,target->source);
    
    fread(&(target->file->reverse1),2,1,target->source);
    fread(&(target->file->reverse2),2,1,target->source);

    fread(&(target->file->offbit),4,1,target->source);

    fread(&(target->info->size),4,1,target->source);
    fread(&(target->info->width),4,1,target->source);
    fread(&(target->info->height),4,1,target->source);
    fread(&(target->info->planes),2,1,target->source);
    if(target->info->planes != 1)return 0;
    fread(&(target->info->bitcount),2,1,target->source);
    fread(&(target->info->compress),4,1,target->source);
    fread(&(target->info->BMPsize),4,1,target->source);
    fread(&(target->info->xpixel),4,1,target->source);
    fread(&(target->info->ypixel),4,1,target->source);
    fread(&(target->info->coloruse),4,1,target->source);
    fread(&(target->info->colorImportance),4,1,target->source);
    if(target->info->height < 0) target->info->direction = 0;
    else target->info->direction = 1;
    char temp[200];
    fread(temp,target->info->size - 40,1,target->source);
    //showBMPInfo(target);
    return 1;

}

void showBMPInfo(BMP * target){
    printf("File ID : %c%c\n",target->file->type[0],target->file->type[1]);
    printf("File Size : %ld\n",(int64_t)target->file->size);
    printf("File reserve1 : %ld\n",(int64_t)target->file->reverse1);
    printf("File reserve2 : %ld\n",(int64_t)target->file->reverse2);
    printf("File offbit : %ld\n",(int64_t)target->file->offbit);
    printf("Info Header Size : %ld\n",(int64_t)target->info->size);
    printf("Width : %ld\n",(int64_t)target->info->width);
    printf("Height : %ld\n",(int64_t)target->info->height);
    printf("Planes : %ld\n",(int64_t)target->info->planes);
    printf("Bit Per Pixel : %ld\n",(int64_t)target->info->bitcount);
    printf("Compress : %ld\n",(int64_t)target->info->compress);
    printf("BMPSize : %ld\n",(int64_t)target->info->BMPsize);
    printf("X_Pixel : %ld\n",(int64_t)target->info->xpixel);
    printf("Y_Pixel : %ld\n",(int64_t)target->info->ypixel);
    printf("Used Color : %ld\n",(int64_t)target->info->coloruse);
    printf("Important Color : %ld\n",(int64_t)target->info->colorImportance);
    
}