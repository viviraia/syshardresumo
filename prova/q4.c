// Prova Modelo SisHard - Questao 4 (2,5)
// Leia o enunciado em prova.html antes de comecar!
// Compile: gcc -Wall -g q4.c -o q4 -pthread
// Uso:     ./q4 <max_threads> <arquivo>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
// inclua o que mais precisar aqui!

typedef struct {
    int id;        // numero da linha (0, 1, 2, ...)
    char *linha;   // texto da linha (sem '\n')
    long soma;     // resultado: a thread preenche aqui
    // adicione campos se precisar (semaforo, mutex, ...)
} t_arg;

long total_geral = 0; // soma de TODAS as linhas (compartilhada!)

// Soma os inteiros de uma linha. Demora 1 segundo. NAO altere.
long soma_linha(char *linha) {
    sleep(1);
    long soma = 0;
    char *p = linha;
    char *fim;
    while (1) {
        long v = strtol(p, &fim, 10);
        if (p == fim) break;
        soma += v;
        p = fim;
    }
    return soma;
}

void *soma_thread(void *_arg) {
    t_arg *arg = _arg;
    arg->soma = soma_linha(arg->linha);
    total_geral += arg->soma; // cuidado aqui!
    return NULL;
}

// Le a proxima linha de fd (sem o '\n'). Retorna NULL no fim do arquivo.
// Use APENAS read. A linha pode ter qualquer tamanho!
char *read_line(int fd) {
    // Seu codigo aqui!
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("USO: ./q4 <max_threads> <arquivo>\n");
        return 1;
    }
    int max_threads = atoi(argv[1]);
    int fd = open(argv[2], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Complete: para cada linha lida, crie uma thread soma_thread.
    // No final, espere todas, imprima o resultado de cada linha e o total.

    return 0;
}
