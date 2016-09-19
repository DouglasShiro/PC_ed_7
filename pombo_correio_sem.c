/**
 * Resolvendo por semáforos
 * Problema dos pombos
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10
#define USER 5
#define TRUE 1

void* usuario(void* arg);
void* pombo(void* arg);
void escreve_carta(int i);
void entrega_carta(int i);

sem_t vaga;
sem_t cheia;
sem_t mutex;

int conta_carta = 0;

int main(){

	int i;
	int *id;

	pthread_t u[USER];
	pthread_t p;

    sem_init(&vaga, 0, N);
    sem_init(&cheia, 0, 0);
    sem_init(&mutex, 0, 1);

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

void* usuario(void* arg){
    int i = *((int*)arg);

	while(TRUE){
		escreve_carta(i);

		sem_wait(&vaga);
		sem_wait(&mutex);

		conta_carta++;

		if(conta_carta == N){
			sem_post(&cheia);
		}
	}
}

void* pombo(void* arg){
    int i;

	while(TRUE){
		sem_wait(&cheia);
		sem_wait(&mutex);

		for(i = 0; i < N; i++){
			entrega_carta(i);
			sem_post(&vaga);
		}
		sem_post(&mutex);
	}
}

void escreve_carta(int i){
    printf("%d postando carta\n", i);
    return;
}

void entrega_carta(int i){
    printf("Carta %d sendo entregue\n", i);
    return;
}
