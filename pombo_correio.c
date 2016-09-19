/**
 * Problema dos pombos
 * U[] -> Pombo -> Destino
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define USER 6
#define TRUE 1
#define CARGA 5

pthread_mutex_t msg = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t usuarios = PTHREAD_COND_INITIALIZER;
pthread_cond_t pombo_correio = PTHREAD_COND_INITIALIZER;

void* usuario(void *arg);
void* pombo(void *arg);

int carta = 0;

int main(){

	int i;
	int *id;

	pthread_t u[USER];
	pthread_t p;

	for(i = 0; i < USER; i++){
		id = (int*)malloc(sizeof(int));
		*id = i;

		pthread_create(&u[i], NULL, usuario, (void*)(id));
	}

	pthread_create(&p, NULL, pombo, NULL);

	pthread_join(u[0], NULL);
	pthread_join(p, NULL);

	return 0;
}

void* usuario(void *arg){
	int i = *((int*)arg);

	while(TRUE){
		pthread_mutex_lock(&msg);
		if(carta >= CARGA-1){
			printf("Carga do pombo comleta: %d de %d\n", carta+1, CARGA);
			pthread_cond_signal(&pombo_correio);
		}
		printf("%d postando carta\n", i);
		carta++;
		while(carta >= CARGA-1){
			printf("Cartas sendo entregue espere pombo voltar\n");
			pthread_cond_wait(&usuarios, &msg);
		}
		pthread_mutex_unlock(&msg);
		printf("carta %d postada\n", i);
		sleep(1);
	}
}

void* pombo(void *arg){

	while(TRUE){
		pthread_mutex_lock(&msg);
		while(carta <= CARGA-1){
			printf("espere carga completa: %d carta(s) de %d\n", carta+1, CARGA);
			pthread_cond_wait(&pombo_correio, &msg);
		}
		printf("Entregar cartas...\n");
		sleep(3);
		carta = 0;
		printf("Cartas entregues voltar para buscar mais\n");
		sleep(1);
		pthread_cond_broadcast(&usuarios);
		pthread_mutex_unlock(&msg);

	}
}
