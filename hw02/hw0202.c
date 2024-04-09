#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _T{
    char team_name[100];
    int team_name_size;
    int WDL_count[3];
    int GFGA_count[2];
    int Pts;
} _team;

static char file_path[1025];
static _team teams[100];
static int team_size = 0;
static int DEBUG_Value = 0;

void read_file();
void get_file_path();
void ERROR_NoFile();
void analyze_input();
void initial_struct_team();
void sort_teams();
void iniTeams();
void output_answer();
int get_index(char * string_target);

int main(){

    get_file_path();
    iniTeams();
    read_file();
    sort_teams();
    output_answer();
    //printf("%d",DEBUG_Value);

}

void output_answer(){

    printf("    Team              W    D    L    GF    GA    GD    Pts\n");
    for(int i=0;i<team_size;i++){
        if((i+1)<10)printf("0%d) ",i+1);
        else printf("%d) ",i+1);
        printf("%-18s",teams[i].team_name);
        //for(int j=strlen(teams[i].team_name);j<16;j++)printf(" ");
        for(int j=0;j<3;j++)printf("%-5d",teams[i].WDL_count[j]);
        for(int j=0;j<2;j++)printf("%-6d",teams[i].GFGA_count[j]);
        if(teams[i].GFGA_count[0] >= teams[i].GFGA_count[1])
        printf("+%-6d",teams[i].GFGA_count[0] - teams[i].GFGA_count[1]);
        else printf("%-7d",teams[i].GFGA_count[0] - teams[i].GFGA_count[1]);
        printf("%d\n",teams[i].Pts);
    }

}

void sort_teams(){
    for(int i=0;i<team_size;i++)teams[i].Pts = teams[i].WDL_count[0] * 3 + teams[i].WDL_count[1];
    for(int i=0;i<team_size;i++){
        for(int j=i+1;j<team_size;j++){
            if(teams[j].Pts > teams[i].Pts){
                _team temp;
                temp = teams[j];
                teams[j] = teams[i];
                teams[i] = temp;
            }
            else if(teams[j].Pts == teams[i].Pts){
                if(teams[j].GFGA_count[0] - teams[j].GFGA_count[1]> teams[i].GFGA_count[0] - teams[i].GFGA_count[1]){
                    _team temp;
                temp = teams[j];
                teams[j] = teams[i];
                teams[i] = temp;
                }
                else if(teams[j].GFGA_count[0] - teams[j].GFGA_count[1] == teams[i].GFGA_count[0] - teams[i].GFGA_count[1]){
                    if(teams[j].GFGA_count[0]>teams[i].GFGA_count[0]){
                        _team temp;
                temp = teams[j];
                teams[j] = teams[i];
                teams[i] = temp;
                    }
                }
            }
        }
    }
}

void iniTeams(){
    for(int i=0;i<100;i++)initial_struct_team(&teams[i]);
}

void analyze_input(char *input){
    //ignore,h_team,c_team,h_score,c_score
    while(*input!=',')input++;
    input++;
    _team input_team[2];
    int input_score[2] = {0};
    initial_struct_team(&input_team[0]);
    initial_struct_team(&input_team[1]);
    
    while(*input!=','){
        input_team[0].team_name[input_team[0].team_name_size] = *input;
        input_team[0].team_name_size += 1;
        input ++;
    }
    input ++;
    while(*input!=','){
        input_team[1].team_name[input_team[1].team_name_size] = *input;
        input_team[1].team_name_size += 1;
        input ++;
    }
    input ++;
    while(*input!=','){
        input_score[0] = input_score[0] * 10 + *input - '0';
        input++;
    }
    input++;
    while(*input!=','){
        input_score[1] = input_score[1] * 10 + *input - '0';
        input++;
    }
    //
    
    int input_team_idx[2];
    for(int i=0;i<2;i++)input_team_idx[i] = get_index(input_team[i].team_name);
    teams[input_team_idx[0]].GFGA_count[0] += input_score[0];
    teams[input_team_idx[0]].GFGA_count[1] += input_score[1];
    teams[input_team_idx[1]].GFGA_count[0] += input_score[1];
    teams[input_team_idx[1]].GFGA_count[1] += input_score[0];
    //printf("%d %d %d %d",input_team_idx[0],input_team_idx[1],input_score[0],input_score[1]);
    if(input_score[0] > input_score[1]){
        teams[input_team_idx[0]].WDL_count[0] ++;
        teams[input_team_idx[1]].WDL_count[2] ++;
    }
    else if(input_score[0] == input_score[1]){
        teams[input_team_idx[0]].WDL_count[1] ++;
        teams[input_team_idx[1]].WDL_count[1] ++;
    }
    else{
        teams[input_team_idx[0]].WDL_count[2] ++;
        teams[input_team_idx[1]].WDL_count[0] ++;
    }
    

}

int get_index(char * string_target){

    for(int i=0;i<team_size;i++){
        if(strcmp(string_target,teams[i].team_name) == 0){
            return i;
        }
    }
    
    strcpy(teams[team_size].team_name,string_target);
    team_size ++;
    return team_size-1;

}

void initial_struct_team(_team * target){

    for(int i=0;i<100;i++)target->team_name[i] = 0;
    for(int i=0;i<2;i++) target->GFGA_count[i] = 0;
    for(int i=0;i<3;i++) target->WDL_count[i] = 0;
    target->team_name_size = 0;
    target->Pts = 0;

}

void read_file(){

    FILE *open_file = fopen(file_path,"r");
    if(open_file == NULL){
        ERROR_NoFile();
        return;
    }
    char input[1000];
    int lines=0;
    while(fgets(input,1000,open_file)!=NULL){
        
        lines++;
        if(!(lines-1))continue;
        analyze_input(input);
     
    }

    
    fclose(open_file);
}

void ERROR_NoFile(){
    printf("File Not Exist!\n");
}

void get_file_path(){
    printf("Please enter the data file name: ");
    fgets(file_path,1025,stdin);
    file_path[strlen(file_path)-1] = file_path[strlen(file_path)-1] == 10 ? 0 : file_path[strlen(file_path)-1];
    //printf("%s",file_path);
}
