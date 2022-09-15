/*
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(int argc, char **argv) {
    int pid;
    sscanf(argv[1], "%d", &pid);
    printf("pid = %d\n", pid);

    char buf[1000];
    sprintf(buf, "/proc/%d/stat", pid);
    FILE *f = fopen(buf, "r");

    int n;
    char name[1000];
    char stat;
    int padre;
    fscanf(f, "%d %s %c %d", &n, name, &stat, &padre);
    printf("name = %s\n", name);
    printf("stato= %c\n", stat);
    printf("padre = %d\n", padre);
    fclose(f);
}
*/

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
typedef struct {
	int pid; //nell'indice 0 c'è la dim dell'array
	char name[100];
	char state;
	float cpu;
	float mem;
}processi;



struct pstat {
	char name[100];
	char state;
    long unsigned int utime;
    long unsigned int stime;
    long int rss; 
    long long unsigned int starttime;
};

void CPUeMEM(processi* p, float memoria_totale){
	//apro /proc/pid/stat
	char buf[1000];
    sprintf(buf, "/proc/%d/stat", p->pid);
    FILE *f = fopen(buf, "r");
    
    struct pstat* result=(struct pstat*)calloc(1,sizeof(struct pstat));
    
    fscanf(f, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %llu %*u %ld",
                result->name, &result->state, &result->utime,&result->stime, &result->starttime, &result->rss);
    
    long long unsigned int total_time;
    total_time=result->utime+result->stime;
    
	//printf("%s %c %lu %lu  %ld %llu %llu\n",result->name, result->state, result->utime, result->stime,result->rss, total_time,
	//result->starttime);
	
    //prendo uptime da /proc/uptime
    float uptime;
    sprintf(buf, "/proc/%s", "uptime");
    f = fopen(buf, "r");
    fscanf(f, "%f",&uptime);
    //printf("uptime =%f\n",uptime);
    
    //prendo Hertz
    float clock=sysconf(_SC_CLK_TCK);
    //printf("_SC_CLK_TCK =%f\n",clock);
   
     //calcolo %CPU
    float seconds= uptime-(result->starttime/clock);
    //printf("seconds= %f\n",seconds);
    
    float CPU=100*((total_time/clock)/seconds);
    printf("CPU= %f\n", CPU);
    
    //calcolo %MEM
    float MEM=((result->rss*4)/memoria_totale)*100; //il *4 lo faccio staticamente, 4 è la dimensione di una pagina, l'info si trova in /proc/pid/smaps ma è enorme e devo capire come
     printf("MEM =%f\n",MEM);
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

processi* contaProcessi(DIR* dirp,struct dirent* dp, processi* p,float memoria_totale){
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
		p[i].cpu=0.f;
		p[i].mem=0.f;
		CPUeMEM(&p[i], memoria_totale);
		
		
		}
		
		
	}
return p;

}


void main(){
	
	
	DIR* dirp;
	struct dirent* dp;

	processi* p=(processi*)calloc(1000,sizeof(processi));
	p[0].pid=1000;
	p[0].cpu = -1;
	p[0].mem = -1;
	
	char buf[1000];
	float memoria_totale;
    sprintf(buf, "/proc/%s", "meminfo");
    FILE* f = fopen(buf, "r");
    fscanf(f, "%*s %f",&memoria_totale);
    printf("memoria totale =%f\n",memoria_totale);
	
	p=contaProcessi(dirp, dp, p, memoria_totale);
	/*
	int i=1;	
	while(i<p[0].pid){
		if(p[i].pid!=0){
			printf("il pid del processo è= %d e l'indice è: %d\n", p[i].pid, i); //mi setta uno sbagliato, poi vedo
		}
		++i;
	}
    */
	return;
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH "/proc"

int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;

    fp = fopen(PATH, "r");
    if (fp == NULL)
    {
        perror(PATH);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (getline(&line, &len, fp) != -1)
            printf("%s",line);
        else
        {
            printf("EOF\n");
            sleep(1);
            clearerr(fp);
        }
    }

    if (line)
        free(line);
    return(EXIT_SUCCESS);
}*/
