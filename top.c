#include "top.h"


int main(){

    int pid;
	char buf[1000];
	float memoria_totale;
	float clock;
	int page_size;
	
	
	DIR* dirp=NULL;
	struct dirent* dp=NULL;
	
	struct pollfd pfd = { .fd = 0, .events = POLLIN };
  	set_term_quiet_input();
	
	char* output=(char*)calloc(128000,1);
	
	processi* p=(processi*)calloc(1000,sizeof(processi));
	p[0].pid=1000;	
	
    sprintf(buf, "/proc/%s", "meminfo");
    FILE* f = fopen(buf, "r");
    fscanf(f, "%*s %f",&memoria_totale);
    fclose(f);
    
    clock=sysconf(_SC_CLK_TCK);
    page_size=sysconf(_SC_PAGESIZE)/1000;

    while(1){
    	
    	system("clear");
    	printf("PID           S              CPU                    MEM               COMMAND\n");
	    p=contaProcessi(dirp, dp, p, memoria_totale, clock, page_size);
	    stampa_processi(p, output);

	    if (poll(&pfd, 1, 0)>0) {
      	int c = getchar();
		printf("Key pressed: %c \n", c);
		if (c=='q') return 0;
		else if(c=='k'){
		  	printf("Insert pid: \n");
		  	fscanf(stdin, "%d", &pid);
		  	kill(pid, SIGKILL);
		}else if(c=='t') {
			printf("Insert pid: \n");
		 	fscanf(stdin, "%d", &pid);
		 	kill(pid, SIGTERM);
		}else if(c=='s') {
			printf("Insert pid: \n");
		 	fscanf(stdin, "%d", &pid);
		  	kill(pid, SIGSTOP);
		}else if(c=='r') {
			printf("Insert pid: \n");
		  	fscanf(stdin, "%d", &pid);
		  	kill(pid, SIGCONT);
		} else continue;
		}

        sleep(2);
    }
    	
	return 0;
}

