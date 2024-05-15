#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/types.h>
//#include <sstream>
#include <sys/stat.h>
#include <fcntl.h>

static char pid[1025];
static off_t addr;
int mem;

// ps aux | grep dosbox

char * mergeString(char * a, char * b);

int main(){

    printf("Please enter the PID of dosbox :");
    scanf("%s",pid);
    printf("Please enter the address of dosbox :");
    scanf("%llx",&addr);

    pid[strlen(pid)-1] = pid[strlen(pid)-1] <= 32 ? 0:pid[strlen(pid)-1];
    //addr[strlen(addr)-1] = addr[strlen(addr)-1] <= 32 ? 0:addr[strlen(addr)-1];
    
    //printf("%ld",addr);

    
    while(1){
        mem = open(mergeString("/proc/",mergeString(pid,"/mem")), O_RDWR,0777);
        if(mem < 0){
            printf("./mem not found.\n");
            exit(0);
        }
        
        lseek(mem,addr,atoi(pid));
        int64_t a[4];
        printf("Please enter the max hp of target character");
        scanf("%ld",a[0]);
        printf("Please enter the current hp of target character");
        scanf("%ld",a[1]);
        printf("Please enter the max mp of target character");
        scanf("%ld",a[2]);
        printf("Please enter the current mp of target character");
        scanf("%ld",a[3]);

        

        close(mem);
    }

    
    //read(mem,in,8);
    //printf("%b",in);

    //printf("Done");
    



}

char * mergeString(char * a, char * b){

    char * c = calloc(strlen(a) + strlen(b) + 1,sizeof(char));
    for(int i=0;i<strlen(a);i++) c[i] = a[i];
    for(int i=0;i<strlen(b);i++) c[ strlen(a) + i ] = b[i];
    return c;
}