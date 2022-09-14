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
typedef struct {
	int pid; //nell'indice 0 c'è la dim dell'array
	float cpu;
	float mem;
}processi;



struct pstat {
	char name[100];
	char stato;
    long unsigned int utime_ticks;
    long int cutime_ticks;
    long unsigned int stime_ticks;
    long int cstime_ticks;
    long unsigned int vsize; // virtual memory size in bytes
    long unsigned int rss; //Resident  Set  Size in bytes
    long unsigned int total_time;
};




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

processi* contaProcessi(DIR* dirp,struct dirent* dp, processi* p){
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
		
		
		}
		
		
	}
return p;

}

void prendi_valori(){
	long unsigned int stat[52];
	char buf[1000];
    sprintf(buf, "/proc/%d/stat", 1466);
    FILE *f = fopen(buf, "r");
    long int rss;
    struct pstat* result=(struct pstat*)calloc(1,sizeof(struct pstat));
    fscanf(f, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu"
                "%lu %ld %ld %*d %*d %*d %*d %*u %lu %ld",
                result->name, &result->stato, &result->utime_ticks, &result->stime_ticks,
                &result->cutime_ticks, &result->cstime_ticks, &result->vsize,
                &rss);
    result->total_time=result->utime_ticks+result->stime_ticks;
    
	printf("%s %c %lu %lu %ld %ld %lu %ld %lu \n",result->name, result->stato, result->utime_ticks, result->stime_ticks,
                result->cutime_ticks, result->cstime_ticks, result->vsize,
                rss, result->total_time);
}

void main(){
	
	prendi_valori();
	DIR* dirp;
	struct dirent* dp;

	processi* p=(processi*)calloc(1000,sizeof(processi));
	p[0].pid=1000;
	p[0].cpu = -1;
	p[0].mem = -1;
	

	
	p=contaProcessi(dirp, dp, p);
	
	int i=1;	
	while(i<p[0].pid){
		if(p[i].pid!=0){
			printf("il pid del processo è= %d e l'indice è: %d\n", p[i].pid, i); //mi setta uno sbagliato, poi vedo
		}
		++i;
	}

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
