#include <termios.h>
#include <sys/poll.h>
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

typedef struct{
 	long unsigned int utime;
    long unsigned int stime;
    long int rss; 
    long long unsigned int starttime;
    long long unsigned int total_time;
    float uptime;
    float seconds;
}utilities;

typedef struct {
	int pid; //nell'indice 0 c'è la dim dell'array
	char name[100];
	char state;
	float cpu;
	float mem;
	int flag;
}processi;

void calcolo_uso_memoria(processi* p, float memoria_totale, float clock, int page_size);
int is_int(char* s);
processi* contaProcessi(DIR* dirp,struct dirent* dp, processi* p,float memoria_totale, float clock, int page_size);
void stampa_processi(processi* p, char* output);

