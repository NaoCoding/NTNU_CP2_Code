#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


int main(){
	double starttime = 0;
	char path[1025];
	uint8_t color[3];
	int timeshift;
	double speed;
	printf("Please enter the file name: ");
	fgets(path,1025,stdin);
	if(path[strlen(path)-1] == 10)path[strlen(path)-1] = 0;
	FILE *file;
	if(!(file = fopen(path,"r"))){
		printf("File not exist!\n");exit(0);
	}
	printf("Time Shift ( -10 ~ 10 ): ");
	scanf("%d",&timeshift);
	if(timeshift > 10 || timeshift < -10){
		printf("Time shift out of range!\n");exit(0);
	}
	printf("Speed (0.25,0.5,0.75,1,1.25,1.5,1.75,2): ");
	scanf("%lf",&speed);
	if((int64_t)(speed * 100) % 25 || speed > 2 || speed < 0.25){
		printf("Speed out of range!\n");exit(0);
	}
	char input[1025];
	starttime = -timeshift;
	while(fgets(input,1025,file)){
		//printf("%s\n",input);
		int idx = 0;
		if(input[idx]=='[')continue;
		while(input[idx] != ':' && input[idx]) idx ++;
		if(idx == strlen(input))continue;
		char inf[1025] = {0};
		for(int i=0;i<idx;i++)inf[i] = input[i];
		if(!strcmp(inf,"Style")){
			for(int i=0;i<3;i++){
				while(input[idx]!=',')idx++;
				idx ++;
			}
			char temp[1025] = {0};
			while(input[idx]!=','){
				temp[strlen(temp)] = input[idx];
				idx++;
			}
			if(temp[0]=='&'){
				for(int i=0;i<6;i++){
					if(isupper(temp[strlen(temp)-1-i]))temp[strlen(temp)-1-i] = temp[strlen(temp)-1-i] - 'A' + 'a';
				}
				for(int i=0;i<3;i++){
					color[2-i] = isdigit(temp[strlen(temp)-1-i*2]) == 1 ? temp[strlen(temp)-1-i*2]-'0':temp[strlen(temp)-1-i*2]-'a'+10;
					//printf("%d %d %d\n",color[0],color[1],color[2]);
					color[2-i] += isdigit(temp[strlen(temp)-2-i*2]) == 1 ? (temp[strlen(temp)-2-i*2]-'0') * 16:(temp[strlen(temp)-2-i*2]-'a'+10)*16;
					//printf("%d %d %d\n",color[0],color[1],color[2]);
				}
			}
			else{
				color[0] = ((int64_t)strtod(temp,NULL))/65536;
				color[1] = ((int64_t)strtod(temp,NULL))%65536/256;
				color[2] = ((int64_t)strtod(temp,NULL))%256;
			}
			printf("\e[1;1H\e[2J");
			printf("\033[38;2;%d;%d;%dm",color[2],color[1],color[0]);
			
		}

		if(!strcmp(inf,"Dialogue")){
			//printf("???");
			while(input[idx]!=',')idx++;
			idx ++;
			double next = 0;
			int start = 0;
			for(int i=0;i<2;i++){
				while(input[idx+start]!=':')start++;
				next *= 60;
				next += strtod(input+idx,NULL);
				idx += start;
				start = 0;
				idx ++;
				//printf("%lf\n",next);
			
			}
			next *= 60;
			while(input[idx+start]!=',')start++;
			next += strtod(input+idx,NULL);
			idx += start;
			idx ++;
			//printf("%lf\n",next);
			usleep((next-starttime)*1000000/speed);
			starttime = next;
			next = 0;
			start = 0;
			for(int i=0;i<2;i++){
				while(input[idx+start]!=':')start++;
				next *= 60;
				next += strtod(input+idx,NULL);
				idx += start;
				start = 0;
				idx ++;
			
			}
			next *= 60;
			while(input[idx+start]!=',')start++;
			next += strtod(input+idx,NULL);
			idx+=start;
			idx ++;
			for(int i=0;i<6;i++){
				while(input[idx]!=',')idx++;
				idx++;
			}
			printf("%s",input+idx);
			usleep((next-starttime)*1000000/speed);
			starttime = next;

			printf("\e[1;1H\e[2J");
		}
		//printf("%s",input);
	}

	printf("\033[m");
}
