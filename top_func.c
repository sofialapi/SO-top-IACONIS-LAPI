#include "top.h"

void calcolo_uso_memoria(processi* p, float memoria_totale, float clock, int page_size){
	
	char buf[1000];
	
    utilities* uti=(utilities*)malloc(sizeof(utilities));
    
    sprintf(buf, "/proc/%d/stat", p->pid);
    FILE *f = fopen(buf, "r");
    
    fscanf(f, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %llu %*u %ld",
                p->name, &p->state, &uti->utime, &uti->stime, &uti->starttime, &uti->rss);

    fclose(f); 

    uti->total_time=uti->utime+uti->stime;

    sprintf(buf, "/proc/%s", "uptime");  
    f = fopen(buf, "r");
    fscanf(f, "%f",&uti->uptime);
    fclose(f);
    
    uti->starttime=uti->starttime/clock;
    uti->seconds= uti->uptime-(uti->starttime);  

    p->cpu=((100*uti->total_time/clock)/uti->seconds);

    p->mem=((uti->rss*page_size)/memoria_totale)*100;     
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
}

processi* contaProcessi(DIR* dirp,struct dirent* dp, processi* p,float memoria_totale, float clock, int page_size){
	
	dirp=opendir("/proc");
	if(dirp==NULL){
	 return NULL;
	 }
	 
	 int j=1;
	while ((dp=readdir(dirp))!=NULL){
	
		char* curr_dir = dp->d_name;
	
		if(curr_dir!=NULL && is_int(curr_dir)!=-1){

			//i=atoi(curr_dir);
		
			if(j>(p[0].pid)-1){
				p=(processi*)realloc(p,(p[0].pid*2)*sizeof(processi));
				p[0].pid=p[0].pid*2;			
			}
	
			p[j].pid=atoi(curr_dir);
			p[j].flag=1;
			calcolo_uso_memoria(&p[j], memoria_totale, clock, page_size);
		}
		j++;
	}
	rewinddir(dirp);
	closedir(dirp);
	return p;
}

void set_term_quiet_input(){
	  struct termios tc;
	  tcgetattr(0, &tc);
	  tc.c_lflag &= ~ICANON;
	  tcsetattr(0, TCSANOW, &tc);
	}
	
