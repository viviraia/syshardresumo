// GABARITO - Prova Modelo SisHard - Questao 1
// Compile: gcc -Wall -g q1.c -o q1 -pthread
//
// Dependencias do diagrama:
//   A -> B     A -> D
//   B -> E     C -> E
//   D -> F
//   E -> G     F -> G
//
// Regra: um semaforo (iniciado em 0) por "aresta de saida" de cada parte.
// Quem termina a parte X faz sem_post(sX) uma vez para CADA parte que depende dela.
// Quem depende de X faz sem_wait(sX) antes de executar.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    sem_t sA, sB, sC, sD, sE, sF;
} t_arg;

void *thread1(void *_arg) {
    t_arg *arg = _arg;
    printf("A\n");
    sem_post(&arg->sA);   // libera B
    sem_post(&arg->sA);   // libera D

    sem_wait(&arg->sB);   // E depende de B
    sem_wait(&arg->sC);   // E depende de C
    printf("E\n");
    sem_post(&arg->sE);   // libera G
    return NULL;
}

void *thread2(void *_arg) {
    t_arg *arg = _arg;
    sem_wait(&arg->sA);   // B depende de A
    printf("B\n");
    sem_post(&arg->sB);
    return NULL;
}

void *thread3(void *_arg) {
    t_arg *arg = _arg;
    printf("C\n");        // C nao depende de ninguem
    sem_post(&arg->sC);

    sem_wait(&arg->sD);   // F depende de D
    printf("F\n");
    sem_post(&arg->sF);
    return NULL;
}

void *thread4(void *_arg) {
    t_arg *arg = _arg;
    sem_wait(&arg->sA);   // D depende de A
    printf("D\n");
    sem_post(&arg->sD);
    return NULL;
}

void *thread5(void *_arg) {
    t_arg *arg = _arg;
    sem_wait(&arg->sE);   // G depende de E
    sem_wait(&arg->sF);   // G depende de F
    printf("G\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    void *(*funcs[5])(void *) = {thread1, thread2, thread3, thread4, thread5};
    pthread_t tids[5];

    t_arg *arg = malloc(sizeof(t_arg));
    sem_init(&arg->sA, 0, 0);
    sem_init(&arg->sB, 0, 0);
    sem_init(&arg->sC, 0, 0);
    sem_init(&arg->sD, 0, 0);
    sem_init(&arg->sE, 0, 0);
    sem_init(&arg->sF, 0, 0);

    for (int i = 0; i < 5; i++) {
        pthread_create(&tids[i], NULL, funcs[i], arg);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(tids[i], NULL);
    }

    sem_destroy(&arg->sA);
    sem_destroy(&arg->sB);
    sem_destroy(&arg->sC);
    sem_destroy(&arg->sD);
    sem_destroy(&arg->sE);
    sem_destroy(&arg->sF);
    free(arg);
    return 0;
}
