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


typedef struct {
	int pid; //nell'indice 0 c'è la dim dell'array
	char name[100];
	char state;
	float cpu;
	float mem;
}processi;




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
		calcolo_uso_memoria(&p[i], memoria_totale, clock, page_size);
		
		if(p[i].pid!=0 && (p[i].state=='D' || p[i].state=='I' || p[i].state=='R' || p[i].state=='S' || p[i].state=='T' ||
		    p[i].state=='t' || p[i].state=='Z')){
			    printf("%d             %c              %.2f               %.2f          %s\n", p[i].pid, p[i].state, p[i].cpu,
			    p[i].mem, p[i].name); 
			    p[i].pid=0; p[i].state='\0'; p[i].cpu=0; p[i].mem=0; 
		    }
		

		}
		
		
	}

rewinddir(dirp);
closedir(dirp);


return p;

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
	
	processi* p=(processi*)calloc(1000,sizeof(processi));
	p[0].pid=1000;
	
	
    sprintf(buf, "/proc/%s", "meminfo");
    FILE* f = fopen(buf, "r");
    fscanf(f, "%*s %f",&memoria_totale);
    fclose(f);
    
    //prendo Hertz
    clock=sysconf(_SC_CLK_TCK);
    page_size=sysconf(_SC_PAGESIZE);
   
   
    printf("PID           S              CPU                 MEM\n");
    while(1){
	
	    p=contaProcessi(dirp, dp, p, memoria_totale, clock, page_size);
	    
		input=getc(stdin);
		
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

        sleep(3);
    }
	return;
}

