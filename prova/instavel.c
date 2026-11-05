// Programa "instavel" usado na Questao 2 da Prova Modelo.
// NAO altere este arquivo. Compile: gcc -Wall instavel.c -o instavel
//
// Uso: ./instavel <tarefa> <n>
//   - retorna -1 se a quantidade de argumentos estiver errada ou se n < 0
//   - caso contrario, simula um servico instavel:
//       ~35% das vezes: sucesso (retorna 0)
//       ~45% das vezes: falha temporaria (retorna 1, 2 ou 3)
//       ~20% das vezes: trava e morre com SIGABRT (abort)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "uso: %s <tarefa> <n>\n", argv[0]);
        return -1;
    }
    long n = atol(argv[2]);
    if (n < 0) {
        fprintf(stderr, "[instavel] n negativo!\n");
        return -1;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec ^ (getpid() << 16));

    int r = rand() % 100;
    usleep(200000); // simula processamento (0,2 s)

    if (r < 35) {
        printf("[instavel] tarefa '%s' processada: %ld^2 = %ld\n", argv[1], n, n * n);
        return 0;
    } else if (r < 80) {
        int erro = 1 + rand() % 3;
        printf("[instavel] falha temporaria (codigo %d)\n", erro);
        return erro;
    } else {
        printf("[instavel] travou!\n");
        fflush(stdout);
        abort();
    }
}
