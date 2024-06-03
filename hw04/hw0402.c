#include <wchar.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>

static int search_method = -1; // 1 : uid // 2 : en
CURL *curl;
FILE * json;
static char UID[1024];
static wchar_t CN[1025];
static uint64_t JSON_length;
static int shouldOutput [2000];

void get_json();

void display_help();

void getStationByUid();
void getStationByEn();
void getStationByCn();

size_t writeInJson();

uint64_t getJsonLength();
char * JQ_Response(char * target);
char * mergeString(char * a, char * b);
void showStationInfo(uint64_t id);
uint8_t subStringIn(char * target);
uint8_t subStringInCN(wchar_t * target);



int main(int argc, char * argv[]){

    struct option optOption[] = {
        {"help",0,NULL,'h'},
        {"uid",1,NULL,'u'},
        {"en",1,NULL,'e'},
        {"cn",1,NULL,'c'}
    };
    int32_t optin;
    setlocale(LC_ALL, "C.UTF-8");
    if(argc <= 1) display_help();
    while((optin = getopt_long(argc,argv,"hu:e:c:",optOption,NULL)) != -1){
        switch(optin){
            case 'h':
                display_help();
                break;
            case 'u':
                if(search_method == -1) search_method = 1;
                else display_help();

                strcpy(UID,optarg);

                break;
            case 'c':
                if(search_method == -1) search_method = 3;
                else display_help();
                swprintf(CN,1025,L"%s",optarg);
                break;
            case 'e':
                if(search_method == -1) search_method = 2;
                else display_help();

                strcpy(UID,optarg);

                break;
            default:
                display_help();
                break;

        }
    }
    //printf("%d",argc);
    if(search_method == -1)display_help();
    
    get_json();
    JSON_length = getJsonLength();
    if(search_method == 1) getStationByUid();
    else if(search_method == 2) getStationByEn();
    else getStationByCn();


    system("rm temp.json");
    
}

uint64_t getJsonLength(){

    FILE * fp = popen("jq \". | length\" temp.json","r");
    uint64_t ans;
    char in[1025];
    fgets(in,1025,fp);
    ans = atoi(in);
    printf("共找到 %lu 個 YouBike 站點\n",ans);
    fclose(fp);
    return ans;


}

uint8_t subStringIn(char * target){
    for(int i=0;i<strlen(target);i++){
        uint32_t match = 0;
        for(int j=0;j<strlen(UID);j++){
            char a = target[i+j];
            if(target[i+j] >= 'a' && target[i+j] <= 'z') a += 'A' - 'a';
            char b = UID[j];
            if(UID[j] >= 'a' && b <= 'z') b += 'A' - 'a';
            if(a == b)match ++;
            else break;
        }
        if(match == strlen(UID)) return 1;
    }
    return 0;
}

uint8_t subStringInCN(wchar_t * target){
    for(int i=0;i<wcslen(target);i++){
        uint32_t match = 0;
        for(int j=0;j<wcslen(CN);j++){
            if(CN[j] == target[i+j])match ++;
            else break;
        }
        if(match == wcslen(CN)) return 1;
    }
    return 0;
}


void getStationByCn(){
    printf("正在搜尋 中文站名 : %ls\n",CN);
    uint8_t total = 0;
    FILE * fp = popen(mergeString("jq \"",mergeString(".[] | .sna","\" temp.json")),"r");
    char *in = calloc(1025,sizeof(char));
    FILE * fp2 = popen(mergeString("jq \"",mergeString(".[] | .ar","\" temp.json")),"r");
    char *in2 = calloc(1025,sizeof(char));
    
    
    for(uint64_t i = 0; i<JSON_length;i++){
        fgets(in,1025,fp);
        fgets(in2,1025,fp2);

        in[strlen(in)-2] = 0;
        in2[strlen(in2)-2] = 0;
        wchar_t ssno[1025],ssno2[1025];
        swprintf(ssno,1025,L"%s",in+1);
        swprintf(ssno2,1025,L"%s",in2+1);
        if(subStringInCN(ssno) || subStringInCN(ssno2)){
            shouldOutput[total++] = i;
        }
        
    }
    if(!total){
        system("clear");
        printf("未搜尋到任何中文站名包含 %ls 的站點\n\n",CN);
    }
    
    

    else{
        printf("\n共 %u 筆資料\n",total);
        if(total > 5) printf("以下只顯示前五筆資料\n\n");
        for(int i=0;i<(total >= 5 ? 5 : total);i++){
            showStationInfo(shouldOutput[i]);
        }
    }

}

void getStationByEn(){

    
    printf("正在搜尋 英文站名 : %s\n",UID);
    uint8_t total = 0;
    FILE * fp = popen(mergeString("jq \"",mergeString(".[] | .snaen","\" temp.json")),"r");
    char *in = calloc(1025,sizeof(char));
    FILE * fp2 = popen(mergeString("jq \"",mergeString(".[] | .aren","\" temp.json")),"r");
    char *in2 = calloc(1025,sizeof(char));
    
    
    for(uint64_t i = 0; i<JSON_length;i++){
        fgets(in,1025,fp);
        fgets(in2,1025,fp2);

        in[strlen(in)-2] = 0;
        in2[strlen(in2)-2] = 0;

        if(subStringIn(in) || subStringIn(in2)){
            shouldOutput[total++] = i;
        }
        
    }
    if(!total){
        system("clear");
        printf("未搜尋到任何英文站名包含 %s 的站點\n\n",UID);
    }
    
    

    else{
        printf("\n共 %u 筆資料\n",total);
        if(total > 5) printf("以下只顯示前五筆資料\n\n");
        for(int i=0;i<(total >= 5 ? 5 : total);i++){
            showStationInfo(shouldOutput[i]);
        }
    }

}


char * JQ_Response(char * target){
    FILE * fp = popen(mergeString("jq \"",mergeString(target,"\" temp.json")),"r");
    char *in = calloc(1025,sizeof(char));
    fgets(in,1025,fp);
    pclose(fp);
    return in;
}

void getStationByUid(){

    
    printf("正在搜尋 UID : %s\n",UID);
    uint8_t total = 0;
    FILE * fp = popen(mergeString("jq \"",mergeString(".[] | .sno","\" temp.json")),"r");
    char *in = calloc(1025,sizeof(char));
    
    
    for(uint64_t i = 0; i<JSON_length;i++){
        fgets(in,1025,fp);

        in[strlen(in)-2] = 0;
        if(!strcmp(UID,in+1)){
            shouldOutput[total++] = i;
            break;
        }
        
    }
    if(!total){
        system("clear");
        printf("未搜尋到任何UID為 %s 的站點\n\n",UID);
    }
    
    

    else{
        printf("\n共 %u 筆資料\n",total);
        if(total > 5) printf("以下只顯示前五筆資料\n\n");
        for(int i=0;i<(total >= 5 ? 5 : total);i++){
            showStationInfo(shouldOutput[i]);
        }
    }

}

void showStationInfo(uint64_t id){

    char s[100] = {0};
    sprintf(s,"%lu",id);

    char * in = JQ_Response(mergeString(".[",mergeString(s,"].sno")));
    in[strlen(in)-2] = 0;
    printf("UID: %s\n",in+1);
     free(in);

    in = JQ_Response(mergeString(".[",mergeString(s,"].sna")));
    in[strlen(in)-2] = 0;
    printf("中文站名： %s\n",in+1);
     free(in);

    in = JQ_Response(mergeString(".[",mergeString(s,"].snaen")));
    in[strlen(in)-2] = 0;
    printf("英文站名： %s\n",in+1);
     free(in);

    in = JQ_Response(mergeString(".[",mergeString(s,"].ar")));
    in[strlen(in)-2] = 0;
    printf("中文地理位置： %s\n",in+1);

    free(in);
    in = JQ_Response(mergeString(".[",mergeString(s,"].aren")));
    in[strlen(in)-2] = 0;
    printf("英文地理位置： %s\n",in+1);

    free(in);
    in = JQ_Response(mergeString(".[",mergeString(s,"].total")));
    in[strlen(in)-1] = 0;
    printf("總腳踏車格子數： %s\n",in);

    free(in);
    in = JQ_Response(mergeString(".[",mergeString(s,"].available_rent_bikes")));
    in[strlen(in)-1] = 0;
    printf("剩餘腳踏車數量： %s\n",in);

    free(in);
    in = JQ_Response(mergeString(".[",mergeString(s,"].available_return_bikes")));
    in[strlen(in)-1] = 0;
    printf("剩餘可停空格數量： %s\n",in);


    free(in);
    in = JQ_Response(mergeString(".[",mergeString(s,"].srcUpdateTime")));
    in[strlen(in)-2] = 0;
    printf("上次系統更新時間： %s\n",in+1);

    free(in);
    in = JQ_Response(mergeString(".[",mergeString(s,"].infoTime")));
    in[strlen(in)-2] = 0;
    printf("上次資訊更新時間： %s\n",in+1);



    printf("\n");

}

void get_json(){

    system("clear");
    printf("YouBike Data Downloading...\n");
    printf("YouBike 資料下載中...\n");

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    json = fopen("temp.json","wb");
    curl_easy_setopt(curl, CURLOPT_URL ,"https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeInJson);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, json);
    
    CURLcode curlcode = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(json);

    system("clear");

}

size_t writeInJson(void *ptr, size_t size, size_t nmemb, FILE *stream){
    size_t w = fwrite(ptr, size, nmemb, stream);
    return w;
}


void display_help(){
    
    printf("YouBike Station Search App\n");
    printf("------------------------------------------------\n");
    printf("Usage: ./hw0402 [options]\n");
    printf("    -h , --help : display help message\n");
    printf("    -u , --uid : search YouBike station by UID (optarg required)\n");
    printf("    -e , --en : search YouBike station by English Name(optarg required)\n");
    printf("    -c , --cn : search YouBike station by Chinese Name(optarg required)\n");

    printf("------------------------------------------------\n");
    printf("使用方法 : ./hw0402 [ 選項 ]\n");
    printf("    -h , --help : 顯示說明/幫助訊息\n");
    printf("    -u , --uid : 使用UID搜尋YouBike站牌 (參數必填)\n");
    printf("    -e , --en : 使用英文站名搜尋YouBike站牌 (參數必填)\n");
    printf("    -c , --cn : 使用中文站名搜尋YouBike站牌 (參數必填)\n");
    exit(0);

}

char * mergeString(char * a, char * b){

    char * c = calloc(strlen(a) + strlen(b) + 1,sizeof(char));
    for(int i=0;i<strlen(a);i++) c[i] = a[i];
    for(int i=0;i<strlen(b);i++) c[ strlen(a) + i ] = b[i];
    return c;
}