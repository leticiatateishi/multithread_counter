#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_WORKERS 4
#define ATIVO 1
#define INATIVO 0

typedef struct {
  int NUMERO;
  int ID;
} thread_args;

pthread_mutex_t trava;
int n_workers = 0;
int n_primos = 0;
pthread_t workers[MAX_WORKERS];
int worker_status[MAX_WORKERS];

int primo(int numero){
    if (numero < 2)
        return 0;
    for (int i = 2; i <= (numero/2); i++){
        if (numero%i == 0)
            return 0;
    }
    return 1;
}


void* worker(void *arg) {
	thread_args *info = (thread_args *)arg;
	pthread_mutex_lock(&trava);
	n_primos += primo(info->NUMERO);
	n_workers --;
	worker_status[info->ID] = INATIVO;
	free(info);
	pthread_mutex_unlock(&trava);
	return NULL;
}

int main() {
	int numero;
	thread_args *send_args;
  	int j;
	while (1){
		if (scanf("%d", &numero) == EOF) 
      		break;
      	if (n_workers >= MAX_WORKERS) 
	      	printf("a\n");
	    else {
			send_args = (thread_args*)malloc(sizeof(thread_args));
      		send_args->NUMERO = numero;

		    /* Procura espaco para thread livre */
		    j = 0;
		    pthread_mutex_lock(&trava);
		    while (worker_status[j] == ATIVO) j++;
		    send_args->ID = j;
		    worker_status[j] = ATIVO;
		    n_workers += 1;
		    // printf("Threads ativas: %d de %d\n", n_workers, MAX_WORKERS);
		    pthread_mutex_unlock(&trava);
		    pthread_create(& (workers[j]), NULL, worker, (void*) send_args);
	    }
	}

	  /* Esperando threads terminarem */
	for (int i = 0; i < MAX_WORKERS; i++) {
		if (worker_status[i]==ATIVO)
    		pthread_join(workers[i], NULL);
	}

	printf("%d\n", n_primos);
  	return 0;
}
