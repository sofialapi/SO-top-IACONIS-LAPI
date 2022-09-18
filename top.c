#include <sys/types.h>
#include <sys/select.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <signal.h>
#include "top.h"
#include <termios.h>
#include <sys/poll.h>


void calcolo_uso_memoria(processi* p, float memoria_totale, float clock, int page_size){
	
	char buf[1000];
    long unsigned int utime;
    long unsigned int stime;
    long int rss; 
    long long unsigned int starttime;
    long long unsigned int total_time;
    float uptime;
    float seconds;
    long int cutime;
    long int cstime;
    
    //apro /proc/pid/stat
    sprintf(buf, "/proc/%d/stat", p->pid);
    FILE *f = fopen(buf, "r");
    
    fscanf(f, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %ld %ld %*d %*d %*d %*d %llu %*u %ld",
                p->name, &p->state, &utime, &stime, &cutime, &cstime, &starttime, &rss);

    fclose(f); 
      
    utime=utime/clock;
    stime=stime/clock;
    cutime=cutime/clock;
    cstime=cstime/clock;
    total_time=utime+stime+cutime+cstime;

    //prendo uptime da /proc/uptime
    sprintf(buf, "/proc/%s", "uptime");  
    f = fopen(buf, "r");
    fscanf(f, "%f",&uptime);
    fclose(f);
    
    starttime=starttime/clock;
    seconds= uptime-(starttime);
    
    //calcolo %CPU
    p->cpu=((100*total_time)/seconds);
    
    
    //calcolo %MEM
    p->mem=((rss*4)/memoria_totale)*100; //il *4 lo faccio staticamente, 4 è la dimensione di una pagina, l'info si trova in /proc/pid/smaps ma è enorme e devo capire come
    
    
}



int is_int(char* s){
	int i=0;
	int len=strlen(s);
	if(s==NULL) return -1;
	while(i<len){
		if((s[i]>=48 && s[i]<=57)){
			i++;
		}else{
			return -1;
		}
	}
	return 1;
}

void stampa_processi(processi* p, char* output){
	int i=1;
	output=strncpy(output,"",128000);
	while(i<p[0].pid){
	if(p[i].pid!=0 && p[i].flag!=0){
		sprintf(output+strlen(output),"%d             %c              %f               %f          %s     \n", p[i].pid, p[i].state,
			 p[i].cpu, p[i].mem, p[i].name);
		}
		p[i].flag=0;
		i++;
	}
	printf("%s\n",output);
	
	/*
	while(i<p[0].pid){
	
		if(p[i].pid!=0 && p[i].flag==1){
			 printf("%d             %c              %f               %f          %s     \n", p[i].pid, p[i].state,
			 p[i].cpu, p[i].mem, p[i].name); 
			 p[i].flag=1;
		}
		i++;
	}
	*/
	
		
}

processi* contaProcessi(DIR* dirp,struct dirent* dp, processi* p,float memoria_totale, float clock, int page_size){
	int i=1;
	dirp=opendir("/proc");
	if(dirp==NULL){
	 return NULL;
	 }

	while ((dp=readdir(dirp))!=NULL){
	
		char* curr_dir = dp->d_name;
	
		if(curr_dir!=NULL && is_int(curr_dir)!=-1){

			i=atoi(curr_dir);
		
			if(i>(p[0].pid)-1){
				p=(processi*)realloc(p,(p[0].pid*2)*sizeof(processi));
				p[0].pid=p[0].pid*2;			
			}
	
			p[i].pid=atoi(curr_dir);
			p[i].flag=1;
			calcolo_uso_memoria(&p[i], memoria_totale, clock, page_size);
		

		}
		
		
	}

rewinddir(dirp);
closedir(dirp);


return p;

}

void set_term_quiet_input(){
	  struct termios tc;
	  tcgetattr(0, &tc);
	  tc.c_lflag &= ~(ICANON | ECHO);
	  tc.c_cc[VMIN] = 0;
	  tc.c_cc[VTIME] = 0;
	  tcsetattr(0, TCSANOW, &tc);
	}
	
void main(){
	char input;
    int pid;
	char buf[1000];
	float memoria_totale;
	float clock;
	int page_size;
	int i=1;
	DIR* dirp;
	struct dirent* dp;
	
	struct pollfd pfd = { .fd = 0, .events = POLLIN };
  	set_term_quiet_input();
	
	char* output=(char*)calloc(128000,1);
	
	processi* p=(processi*)calloc(1000,sizeof(processi));
	p[0].pid=1000;
	
	
    sprintf(buf, "/proc/%s", "meminfo");
    FILE* f = fopen(buf, "r");
    fscanf(f, "%*s %f",&memoria_totale);
    fclose(f);
    
    //prendo Hertz
    clock=sysconf(_SC_CLK_TCK);
    page_size=sysconf(_SC_PAGESIZE);
   
   
    
    while(1){
    	//printf("\033[H\033[J");
    	system("clear");
    	
    	printf("PID           S              CPU                    MEM            COMMAND\n");
	    p=contaProcessi(dirp, dp, p, memoria_totale, clock, page_size);
	    stampa_processi(p, output);
	    
	    if (poll(&pfd, 1, 0)>0) {
      int c = getchar();
      printf("Key pressed: %c \n", c);
      if (c=='q') return;
    }
		/*
		input=getchar();
		
		switch(input){
			case 'q':
				printf("exit\n");
				return;
			case 'k':
				printf("Invio SIGKILL a:\n");
				fscanf(stdin, "%d", &pid);
				kill(pid, SIGKILL);
				break;
			case 't':
				printf("Invio SIGTERM a:\n");
				fscanf(stdin, "%d", &pid);
				kill(pid, SIGTERM);
				break;
			case 's':
				printf("Invio SIGSTOP a:\n");
				fscanf(stdin, "%d", &pid);
				kill(pid, SIGSTOP);
				break;
			case 'r':
				printf("Invio SIGCONT a:\n");
				fscanf(stdin, "%d", &pid);
				kill(pid, SIGCONT);
				break;
			default:
				break;	
		}
		*/

        sleep(2);
    }
	return;
}

