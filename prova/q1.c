// Prova Modelo SisHard - Questao 1 (2,5)
// Leia o enunciado em prova.html antes de comecar!
// Compile: gcc -Wall -g q1.c -o q1 -pthread

// Faca os includes necessarios aqui
#include <stdio.h>

void *thread1(void *_arg) {
    printf("A\n");

    printf("E\n");
    return NULL;
}

void *thread2(void *_arg) {
    printf("B\n");
    return NULL;
}

void *thread3(void *_arg) {
    printf("C\n");

    printf("F\n");
    return NULL;
}

void *thread4(void *_arg) {
    printf("D\n");
    return NULL;
}

void *thread5(void *_arg) {
    printf("G\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    // Crie TODAS as threads em um laco (dica: vetor de ponteiros para funcao).
    // Use semaforos para respeitar o diagrama.

    // Espere por TODAS as threads e libere os recursos.

    return 0;
}
