#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

typedef struct {
	int pid; //nell'indice 0 c'è la dim dell'array
	char name[100];
	char state;
	float cpu;
	float mem;
}processi;



struct pstat {
    long unsigned int utime;
    long unsigned int stime;
    long int rss; 
    long long unsigned int starttime;
};

void CPUeMEM(processi* p, float memoria_totale, float clock){
	//apro /proc/pid/stat
	char buf[1000];
    int ret;
    int controllo;
    controllo = sprintf(buf, "/proc/%d/stat", p->pid);
    if (controllo<0)printf("ERRORE\n");
    FILE *f = fopen(buf, "r");
    
    struct pstat* result=(struct pstat*)calloc(1,sizeof(struct pstat));
    
    controllo = fscanf(f, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %llu %*u %ld",
                p->name, &p->state, &result->utime, &result->stime, &result->starttime, &result->rss);
    if(controllo<0) printf("ERRORE1\n");
    ret=fclose(f);
    if(ret==-1) printf("errore nella fclose\n");
    long long unsigned int total_time;
    total_time=result->utime+result->stime;

    //prendo uptime da /proc/uptime
    float uptime;

    controllo = sprintf(buf, "/proc/%s", "uptime");
    if(controllo<0) printf("ERRORE2\n");
    f = fopen(buf, "r");
    if(f==NULL) printf("%s\n", strerror(errno));

    controllo = fscanf(f, "%f",&uptime);
    if(controllo<0) printf("ERRORE2\n");

    ret=fclose(f);
    if(ret==-1) printf("errore nella fclose\n");

    float seconds= uptime-(result->starttime/clock);
    
    //calcolo %CPU
   
    p->cpu=100*((total_time/clock)/seconds);
    
    //calcolo %MEM
    p->mem=((result->rss*4)/memoria_totale)*100; //il *4 lo faccio staticamente, 4 è la dimensione di una pagina, l'info si trova in /proc/pid/smaps ma è enorme e devo capire come
    free(result);
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

processi* contaProcessi(DIR* dirp,struct dirent* dp, processi* p,float memoria_totale, float clock){
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
		CPUeMEM(&p[i], memoria_totale);
		
		//printf("CPU= %f\n", p[i].cpu);
        //printf("MEM =%f\n",p[i].mem);

		}
		
		
	}

rewinddir(dirp);
int ret=closedir(dirp);
if(ret==-1) printf("errore nella closedir\n");

return p;

}


void main(){
	
	
	DIR* dirp;
	struct dirent* dp;

	processi* p=(processi*)calloc(1000,sizeof(processi));
	p[0].pid=1000;
	
	char buf[1000];
	float memoria_totale;
    sprintf(buf, "/proc/%s", "meminfo");
    FILE* f = fopen(buf, "r");
    fscanf(f, "%*s %f",&memoria_totale);
    printf("memoria totale =%f\n",memoria_totale);

    //prendo Hertz
    float clock=sysconf(_SC_CLK_TCK);
   
   
    int i=1;
    int j=0;
    char c;
    while(1){
    //problema 1: non elimina un pid se il processo muore
    //problema 1: trovare modo per terminare
	
	    p=contaProcessi(dirp, dp, p, memoria_totale, clock);
        printf("\n***************\n");	
	    while(i<p[0].pid){
		    if(p[i].pid!=0){
			    printf("il pid del processo è= %d\n", p[i].pid); 
		    }
		    ++i;
	    }
        i=1;
        sleep(2);
    }
	return;
}

