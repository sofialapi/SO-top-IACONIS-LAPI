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

int getPid(DIR* dirp,struct dirent* dp){
	int num_proc=0;
	dirp=opendir("/proc");
	if(dirp==NULL){
	 printf("niente");
	 return -1;
	 }
	while ((dp=readdir(dirp))!=NULL){
		if(dp->d_name!=NULL && is_int(dp->d_name)!=-1){
			printf("%s\n",dp->d_name);
			num_proc+=1;	
		}
	}
	return num_proc;
}
void main(){
	
	DIR* dirp;
	struct dirent* dp;

	int num_proc=0;
	num_proc=getPid(dirp, dp);
	printf("ci sono= %d processi\n", num_proc);
	closedir(dirp);
	return 0;
}
