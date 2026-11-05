// Prova Modelo SisHard - Questao 2 (2,5)
// Leia o enunciado em prova.html antes de comecar!
// Compile: gcc -Wall -g q2.c -o q2
// Uso:     ./q2 <tarefa> <n>
//
// NAO pode usar system(). Use fork, execvp e wait/waitpid.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("USO: ./q2 <tarefa> <n>\n");
        return 1;
    }

    int tentativa = 0;
    int continuar = 1;

    while (continuar) {
        tentativa++;

        // 1) crie um processo filho
        // 2) no filho, execute ./instavel <tarefa> <n>
        // 3) no pai, espere o filho e trate o resultado (veja o enunciado)

        continuar = 0; // remova esta linha!
    }

    return 0;
}
