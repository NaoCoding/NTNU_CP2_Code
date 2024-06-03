#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/types.h> 
#include <dirent.h>
#include <string.h>
#include <unistd.h>

static double count = -1;
static double uid = -1;
static double time = 5;
static double hertz;

void display_help(){

    printf("Usage: hw0401 [options]\n");
    printf("    -t, --time-interval=time Update the information every [time] seconds. Default: 5 seconds.\n");
    printf("    -c, --count Update the information [count] times. Default: infinite.\n");
    printf("    -p, --pid=pid Only display the given process information.\n");
    printf("    -h, --help Display this information and exit.\n");


    exit(0);

}

int myisdigit(char a[]){
    for(int i=0;i<strlen(a);i++){
        if(!isdigit(a[i])) return 0;
    }
    return 1;
}

void targetFound(char target [] ){

    char buf[1025] = {0};
    strcpy(buf,"/proc/");
    sprintf(buf + strlen(buf) , "%s", target );
    sprintf(buf + strlen(buf) , "%s", "/stat");

    FILE * file = fopen(buf,"r");
    FILE * uptimefile = fopen("/proc/uptime","r");
    char uptime_char[500] = {0};
    fgets(uptime_char,500,uptimefile);
    fclose(uptimefile);
    int idx = 0;
    double uptime = strtod(uptime_char,NULL);

    idx = 0;
    char in[1025];
    fgets(in,1025,file);
    int inidx = 0, tempidx = 0;
    char temp[1025] = {0};
    double t[5] = {0};
    while(idx < 24){
        if(in[inidx] == ' '){
            idx ++;
            if(idx == 1) printf("%-6s%5s",temp,"");
            if(idx == 2){
                for(int i=1;i<strlen(temp)-1;i++)printf("%c",temp[i]);
                for(int i=0;i<42 - strlen(temp)-1;i++)printf(" ");
            }
            if(idx == 3){
                printf("%s%11s",temp,"");
            }
            if(idx == 14) t[0] = strtod(temp,NULL);
            if(idx == 15) t[1] = strtod(temp,NULL);
            if(idx == 16) t[2] = strtod(temp,NULL);
            if(idx == 17) t[3] = strtod(temp,NULL);
            if(idx == 22){
                //printf("%lf %lf %lf %lf",t[0],t[1],t[2],t[3]);
                t[4] = strtod(temp,NULL);
                t[1] += t[0] + t[2] + t[3];
                t[2] = uptime - (t[4] / hertz);
                //printf("%lf %lf %lf %lf",t[4],t[1],t[2],hertz);
                printf("%.2lf%%%3s",100 * t[1]/hertz/t[2],"");
            }
            if(idx == 23)printf("%s",temp);
            


            for(int i=0;i<1025;i++) temp[i] = 0;
            tempidx = 0;
            inidx ++;
        }
        temp[tempidx ++] = in[inidx ++];

    }
    printf("\n");
    fclose(file);

}

void parseLS(){

    DIR * proc = opendir("/proc");
    struct dirent* dr;
    
    while((dr = readdir(proc))){
        if(myisdigit(dr->d_name)){
            if(uid == -1) targetFound(dr->d_name);
            else if(uid == atoi(dr->d_name)){
                targetFound(dr->d_name);
            }
        }
    }
    closedir(proc);


}

void loop(){

    system("clear");
    printf("%-6s%5s%s%40s%5s%5s%5s%s\n","PID","","NAME","STATE","","CPU","","MEM");
    parseLS();
    count -= 1;
    if(count != 0) usleep(1000000 * time);
}

int main(int argc, char * argv[]){

    struct option optOption[] = {
        {"time-interval",1,NULL,'t'},
        {"count",1,NULL,'c'},
        {"pid",1,NULL,'p'},
        {"help",0,NULL,'h'}
    };

    int32_t optin;
    
    while((optin = getopt_long(argc,argv,"hp:c:t:",optOption,NULL)) != -1){

        switch (optin)
        {
        case 'h':
            display_help();
            break;
        case 'c':
            count = strtod(optarg,NULL);
            count += 1;
            break;
        case 'p':
            uid = strtod(optarg,NULL);
            break;
        case 't':
            time = strtod(optarg,NULL);
            if(time<=0) display_help();
            break;

        default:
            display_help();
            break;
        }

    }

    hertz = sysconf(_SC_CLK_TCK);
    
    if(count == -1)while(1)loop();
    else while(count >= 1)loop();

}   