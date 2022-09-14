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

int is_int(char* s){
	int res=0;
	int i=0;
	int len=strlen(s);
	while(i<len){
		if(s[i]>=48 && s[i]<=57){
			res+=1;
			i++;
		}else{
			return -1;
		}
	}
	return res;
}

void contaProcessi(DIR* dirp,struct dirent* dp, processi* p){
	int i=1;
	dirp=opendir("/proc");
	if(dirp==NULL){
	 return;
	 }
	 printf("ciao2\n");
	while ((dp=readdir(dirp))!=NULL){
	printf("ciao3\n");
		if(dp->d_name!=NULL && is_int(dp->d_name)!=-1){
			i=atoi(dp->d_name);
			printf("ciao4\n");
			if(i>p[0].pid-1){
				p=(processi*)realloc(p,p[0].pid*2);
				p[0].pid=p[0].pid*2;
				printf("dimensione %d\n", p[0].pid);
			}
			printf("ciao6\n");
			p[i].pid=atoi(dp->d_name);
			p[i].cpu=0;
			p[i].mem=0;
		}
	}
printf("*******************\n");
}
void main(){
	
	DIR* dirp;
	struct dirent* dp;
	printf("ciao1\n");
	processi* p=(processi*)calloc(100,sizeof(processi));
	p[0].pid=100;
	printf("dimensione %d\n",p[0].pid );
	contaProcessi(dirp, dp, p);
	printf("ciao7\n");
	int i=0;
	while (i<p[0].pid){
		if(p[i].pid!=0){
		printf("processo con pid: %d\n", p[i].pid); 
		
		}i++;
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
