#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

int found(char a[], char b[]){
    int p = 1;
    int r = 0;
    for(int i=0;i<strlen(b);i++){
        for(int j=0;j<strlen(a)&&p;j++){
            char c = isupper(a[j]) ? a[j]-'A'+'a' : a[j] , d = isupper(b[i+j]) ? b[i+j]-'A'+'a' : b[i+j];
            if(c!=d)p=0;
        }
        if(p) return 1;
        p = 1;
        if(b[strlen(a)+i-1]=='\"')r++;
        if(r)break;
    }
    return 0;
}

int main(){
    FILE * bible = fopen("./bible.txt","r");
    if(bible == NULL){
        printf("File not found.\n");
        exit(0);
    }
    char target[5000];
    printf("Please enter the search target: ");
    fgets(target,1025,stdin);
    if(target[strlen(target)-1]==10) target[strlen(target)-1] = 0;
    char in[5000];
    int ans = 0;
    int sum[50000] = {-1};
    int now = 0;
    while(fgets(in,5000,bible)){
        int idx = 0;
        for(int i=0;i<3;i++){
            while(in[idx] != ':')idx++;
            idx ++;
        }
        idx ++;
        if(found(target,in+idx))sum[ans++] = now;
        now ++;
    }
    fclose(bible);
    bible = fopen("./bible.txt","r");
    now = 0;
    int now2 = 0;
    printf("Found %d time(s)\n",ans);
    while(fgets(in,5000,bible) && now2 < ans){
        if(now++ == sum[now2]){
            int a=0,b=0;
            char p[5000]={0},q[5000]={0};
            int pidx = 0,qidx = 0;
            int idx = 0;
            while(in[idx]!=':')idx++;
            idx ++;
            while(isdigit(in[idx++]))a = a * 10 + in[idx-1]-'0';
            while(in[idx]!=':')idx++;
            idx ++;
            while(isdigit(in[idx++]))b = b * 10 + in[idx-1]-'0';
            while(in[idx]!=':')idx++;
            idx +=2;
            while(in[idx]!='\"')p[pidx++]=in[idx++];
            for(int i=0;i<2;i++){
                while(in[idx]!=':')idx++;
                idx++;
            }
            idx +=1;
            while(in[idx]!='\"')q[qidx++]=in[idx++];

            printf("%d. %s %d.%d %s\n",++now2,q,a,b,p);
        } 
    }
    fclose(bible);
}