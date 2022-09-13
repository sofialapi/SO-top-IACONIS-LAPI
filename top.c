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

void contaProcessi(DIR* dirp,struct dirent* dp, int* pid){
	int num_proc=0;
	int i=0;
	dirp=opendir("/proc");
	if(dirp==NULL){
	 return;
	 }
	

	while ((dp=readdir(dirp))!=NULL){
		if(dp->d_name!=NULL && is_int(dp->d_name)!=-1){
			if(i<sizeof(pid)/sizeof(int)){
				pid=(int*)realloc(pid,sizeof(pid));

			}
			pid[i]=	atoi(dp->d_name);
			
			i++;
		}
	}

}
void main(){
	
	DIR* dirp;
	struct dirent* dp;

	int* pid=(int*)malloc(5*sizeof(int));
	contaProcessi(dirp, dp, pid);
	printf("ciao\n");
	printf("primo elem:%d\n", pid[0]);
	int i=0;
	while (pid[i]!=NULL){
		printf("processo con pid: %d\n", pid[i]); //SI ROMPE QUA 
		i++;
	}
	
	closedir(dirp);
	
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