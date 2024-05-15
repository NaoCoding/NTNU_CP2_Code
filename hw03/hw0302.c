#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

static FILE * include_file;
static FILE * read_file[1025];
static int read_file_amount = 0;
static char function[1025];
static int func[2];
static int linumcode[2];
static int allbreak = 0;
static int allbreak2 = 0;

int IsFunction(char * in){
    int pos = 1;
    int inhow[2] = {0,0};
    for(int i=0;i<strlen(in);i++){
        if(in[i] == '\"') inhow[1] = 1;
        if(in[i] == '\'') inhow[0] = 1;
        if(in[i] == in[i+1] && in[i+1] == '/') pos = 0;
        if(in[i] == '/' && in[i+1] == '*')allbreak2 = 1;
        if(allbreak2 && in[i] == '*' && in[i+1] == '/'){
            allbreak2 = 0;
            pos = 1;
        }
        if(allbreak2) pos = 0;
        if(in[i] == '(' && pos && inhow[1]%2 == 0 && inhow[0] %2 == 0) return 1;
    }
    return 0;
}

int appear(char *target, char* source){

    int thisLinebreak = 0;
    for(int i=0;i<strlen(source)-1;i++){
        int valid = 1;
        
        if(source[i] == '/' && source[i+1] == '/') thisLinebreak = 1;
        if(source[i] == '/' && source[i+1] == '*') allbreak = 1;
        if(source[i] == '*' && source[i+1] == '/') allbreak = 0;
        if(allbreak) thisLinebreak = 1;
        for(int j=0;j+i<strlen(source) && j < strlen(target) && !thisLinebreak;j++){
            if(target[j] != source[i+j]){
                valid = 0;
                break;
            }
            
        }
        if(valid&&!thisLinebreak){
            int idx = strlen(target);
            while(source[idx+i]==' ')idx++;
            if(source[idx+i] != '(') return 0;
            return 1;
        }
        
    }
    return 0;

}

void display_help(){

    printf("Usage: hw0302 [options] ... [files] ...\n");
    printf("  -f, --function=func Trace the func usage in [Files].\n");
    printf("  -i, --include=File Trace all functions listed in the header file in [Files].\n");
    printf("  -l, --linum Display the line number.\n");
    printf("  -c, --code Display the code.\n");
    printf("  -h, --help Display this information and exit.\n\n");
    printf("-f and -i are exclusive and must be at least one.\n");

    exit(0);
}

void output(char * buf){    

    int idx = 0;
    while(buf[idx] == ' ')idx ++;
    printf("%s",buf+idx);

}

int main(int argc, char*argv[]){

    function[0] = 0;
    func[0] = 0;
    func[1] = 0;
    linumcode[0] = 0;
    linumcode[1] = 0;
    struct option looooooooogoptions[] = {
        {"help",0,NULL,'h'},
        {"include",1,NULL,'i'},
        {"function",1,NULL,'f'},
        {"linum",0,NULL,'l'},
        {"code",0,NULL,'c'},
    };

    if(argc==1)display_help();

    const char * optstr = "f:i:lch";
    char in;
    opterr = 0;
    while((in = getopt_long(argc,argv,optstr,looooooooogoptions,NULL)) != -1){
        switch(in){
            case 'h':
                display_help();
                break;
            case 'i':
                func[0] = 1;
                //printf("%s",optarg);
                if(!(include_file = fopen(optarg,"r"))){
                    printf("%s Not found\n",optarg);
                    exit(0);
                }
                //printf("%s",optarg);
                break;
            case 'f':
                func[1] = 1;
                strcpy(function,optarg);
                //printf("%s",optarg);
                break;
            
            case 'l':
                linumcode[0] = 1;
                break;
            case 'c':
                linumcode[1] = 1;
                break;

            default:
                printf("The option \"%s\" is not valid\n",argv[optind-1]);
                exit(0);
           
        }
    }
    for(int i=optind;i<argc;i++){
        if(!(read_file[read_file_amount] = fopen(argv[i],"r"))){
            printf("Filename : \"%s\" is not exist. \n",argv[i]);
            exit(0);
        }
        read_file_amount ++;
    }

    //printf("%d %d",func[0],func[1]);
    if(func[1] ==  func[0]){
        printf("-f and -i are exclusive and must be at least one.\n");
        exit(0);
    }

    //return 0;

    if(func[1]){
        printf("%s:\n",function);
        for(int i=0;i<read_file_amount;i++){
            int line = 1;
            int count = 0;
            char buf[3000];
            read_file[i] = fopen(argv[optind+i],"r");
            while(fgets(buf,3000,read_file[i])){
                if(appear(function,buf)){
                    count += 1;
                } 
            }
            printf("  %s (count: %d)\n",argv[optind+i],count);
            if(linumcode[0] + linumcode[1]){
                read_file[i] = fopen(argv[optind+i],"r");
                while(fgets(buf,3000,read_file[i])){
                    if(appear(function,buf)){
                        printf("    ");
                        if(linumcode[0] && !linumcode[1])printf("line %d\n",line);
                        else if(linumcode[0] && linumcode[1]){
                            printf("line %d: ",line);
                            output(buf);
                        }
                        else if(!linumcode[0])output(buf);
                    }
                    line ++;
                }
            }
        }
    }
    else if(func[0]){

        char in[3000];
        while(fgets(in,3000,include_file)){
            in[strlen(in)-1] = in[strlen(in)-1] == 10 ? 0 : in[strlen(in)-1];
            //printf("%s\n",in);
            //continue;
            if(IsFunction(in)){
                for(int i=0;i<strlen(in)-1;i++){
                    if(in[i] == '('){
                        char target[3000] = {0};
                        int kk = i - 1;
                        while(in[kk] == ' ')kk-=1;
                        for(int j=kk;j>=0;j--){
                            if(isalnum(in[j]) || in[j] == '_')target[kk-j] = in[j];
                            else break;
                        }
                        for(int j=0;j<strlen(target)/2;j++){
                            char temp = target[j];
                            target[j] = target[strlen(target)-1-j];
                            target[strlen(target)-1-j] = temp;
                        }
                        if(!strlen(target))break;
                        printf("%s:\n",target);
                        //target[strlen(target)] = '(';
                        for(int i=0;i<read_file_amount;i++){
                            int line = 1;
                            int count = 0;
                            char buf[3000];
                            read_file[i] = fopen(argv[optind+i],"r");
                            while(fgets(buf,3000,read_file[i])){
                                if(appear(target,buf)){
                                    count += 1;
                                } 
                            }

                            //target[strlen(target)-1] = 0;
                            printf("  %s (count: %d)\n",argv[optind+i],count);
                            if(linumcode[0] + linumcode[1]){
                                //target[strlen(target)] = '(';
                                read_file[i] = fopen(argv[optind+i],"r");
                                while(fgets(buf,3000,read_file[i])){
                                    if(appear(target,buf)){
                                        printf("    ");
                                        if(linumcode[0] && !linumcode[1])printf("line %d\n",line);
                                        else if(linumcode[0] && linumcode[1]){
                                            printf("line %d: ",line);
                                            output(buf);
                                        }
                                        else if(!linumcode[0])output(buf);
                                    }
                                    line ++;
                                }
                            }
                        }
                    }
                }
            }
        }

    }




    if(func[0]==1)fclose(include_file);
    for(int i=0;i<read_file_amount;i++)fclose(read_file[i]);

    

}