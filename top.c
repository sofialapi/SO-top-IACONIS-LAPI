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

void main(){

	DIR* dirp;
	struct dirent* dp;
	int num_proc=0;
	
	dirp=opendir("/proc");
	if(dirp==NULL){
	 printf("niente");
	 return -1;
	 }
	while ((dp=readdir(dirp))!=NULL){
		if(dp->d_name!=NULL){
			printf("%s\n",dp->d_name);
			num_proc+=1;
		}
	}
	printf("ci sono= %d processi\n", num_proc);
	closedir(dirp);
	return 0;
}
