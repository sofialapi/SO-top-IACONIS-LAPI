typedef struct {
	int pid; //nell'indice 0 c'è la dim dell'array
	char name[100];
	char state;
	float cpu;
	float mem;
}processi;

void calcolo_uso_memoria(processi* p, float memoria_totale, float clock, int page_size);
int is_int(char* s);
processi* contaProcessi(DIR* dirp,struct dirent* dp, processi* p,float memoria_totale, float clock, int page_size);


