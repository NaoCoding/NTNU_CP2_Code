#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

static float BPM = 0;
static float OFFSET = 0;
static int course = -1;
static int beat = 4,note = 4;
static int start = 0;
static int64_t rq = 0;
static int *b;
static int lenb = 0;
static int courseoutput = 0;
static float now = 0;
void CHECK(char * target);

int main(){

    char *input_ = calloc(1000,sizeof(char));
    while(!feof(stdin)){fgets(input_,1000,stdin);CHECK(input_);}
    free(input_);
    //printf("%f %f",BPM,OFFSET);

}

void CHECK(char * target){
    //printf("1") ;
    if(1){
        if(!(target[0]-'B')){if(!(target[1]-'P')){if(!(target[2]-'M')){
        if(!(target[3]-':'))BPM = strtod(target+4,NULL);//printf("%f",BPM);
        }}}
        if(!(target[0]-'O')){if(!(target[1]-'F')){
        if(!(target[2]-'F')){if(!(target[3]-'S')){
        if(!(target[4]-'E')){if(!(target[5]-'T')){
        if(!(target[6]-':'))OFFSET = strtod(target+7,NULL);
        now = - OFFSET;
        }}}}}}
        if(!(target[0]-'C')){if(!(target[1]-'O')){
        if(!(target[2]-'U')){if(!(target[3]-'R')){
        if(!(target[4]-'S')){if(!(target[5]-'E')){
        if(!(target[6]-':')){if(!(target[7]-'O')){
        if(!(target[8]-'n')){if(!(target[9]-'i')){course = 3;
        }}}
        if(!(target[7]-'H')){if(!(target[8]-'a')){
        if(!(target[9]-'r')){if(!(target[10]-'d')){course = 2;
        }}}}
        if(!(target[7]-'N')){if(!(target[8]-'o')){
        if(!(target[9]-'r')){if(!(target[10]-'m')){
        if(!(target[11]-'a')){if(!(target[12]-'l')){course = 1;
        }}}}}}
        if(!(target[7]-'E')){if(!(target[8]-'a')){
        if(!(target[9]-'s')){if(!(target[10]-'y')) course = 0;
        }}}
        if(!(target[7]-'E')){if(!(target[8]-'d')){
        if(!(target[9]-'i')){if(!(target[10]-'t')) course = 4;
        }}}
        if(!(target[7]-'4')) course = 4;
        if(!(target[7]-'3')) course = 3;
        if(!(target[7]-'2')) course = 2;
        if(!(target[7]-'1')) course = 1;
        if(!(target[7]-'0')) course = 0;
        }}}}}}}
        if(!(target[0]-'#')){if(!(target[1]-'M')){
        if(!(target[2]-'E')){if(!(target[3]-'A')){
        if(!(target[4]-'S')){if(!(target[5]-'U')){
        if(!(target[6]-'R')){if(!(target[7]-'E')){
        int idx = 8;
        while(!isdigit(target[idx])) target ++;
        beat = 0;

        while(isdigit(target[idx])){
            beat *= 10;
            beat += target[idx] - '0';
            idx ++;
        }
        idx ++;
        note = 0;
        while(isdigit(target[idx])&&target[idx]){
            note *= 10;
            note += target[idx] - '0';
            idx ++;
        }

        }}}}}}}
        if(!(target[1]-'B')){if(!(target[2]-'P')){if(!(target[3]-'M'))
        if(!(target[4]-'C')){if(!(target[5]-'H')){if(!(target[6]-'A'))
        {if(!(target[7]-'N')){if(!(target[8]-'G')){if(!(target[9]-'E')){
        BPM = strtod(target+11,NULL);
        //printf("%f",BPM);
        }}}}}}}}
        if(!(target[1]-'S')){if(!(target[2]-'T')){
        if(!(target[3]-'A')){if(!(target[4]-'R')){
        if(!(target[5]-'T')) start = 1;
        }}}}
        if(!(target[1]-'E')){if(!(target[2]-'N')){if(!(target[3]-'D'))start = 0;
            puts("");
        now = -OFFSET;
        courseoutput = 0;}}
        return;
        }

    }
    if(start){
        if(!courseoutput){
            printf("course: %d\n",course);
            courseoutput ++;
        }
        if(!lenb) b = calloc(1000,sizeof(int));
        //printf("%d",lenb);
        //printf("%d",*target);
        //printf("%d",b[0]);
        if(*target==','){
            now += (240/BPM);
            //rintf("%f\n",now);
        }

        else if(isdigit(*target)){
            while(*target){
            while(isdigit(*target)){
                    //printf("%d",*target-'0');
                b[lenb] = *target - '0';
                lenb ++;
                target ++;
            }
            if(*target == ','){

                int mid = 1;
                int m = lenb;
                if(beat > lenb){
                    while(m%lenb && m%beat) m++;
                    mid = m/lenb;
                }


                for(int i=0;i<lenb;i++){
                    if(b[i]<=4 && b[i] >0)
                    printf("[%d, %f]\n",b[i] ,now);
                    now += (60/BPM) * beat  / m * 4 / note * mid;
                }

                lenb = 0;
                break;
            }
            else break;
            //target ++;
        }
        //free(b);
        }
    }

}
