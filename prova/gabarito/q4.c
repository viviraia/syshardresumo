// GABARITO - Prova Modelo SisHard - Questao 4
// Compile: gcc -Wall -g q4.c -o q4 -pthread
// Teste:   time ./q4 2 numeros.txt     (6 linhas, 2 por vez -> ~3 s)
//          valgrind --leak-check=full ./q4 3 numeros.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int id;
    char *linha;
    long soma;
    sem_t *limite;            // limita quantas threads trabalham ao mesmo tempo
    pthread_mutex_t *mutex;   // protege total_geral
} t_arg;

long total_geral = 0;

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

    sem_wait(arg->limite);          // pede uma "vaga" (bloqueia se ja ha max_threads)
    arg->soma = soma_linha(arg->linha);
    sem_post(arg->limite);          // devolve a vaga

    pthread_mutex_lock(arg->mutex); // regiao critica: leitura+escrita da global
    total_geral += arg->soma;
    pthread_mutex_unlock(arg->mutex);
    return NULL;
}

char *read_line(int fd) {
    int cap = 16, len = 0;
    char *buf = malloc(cap);
    char c;
    int lidos;

    while ((lidos = read(fd, &c, 1)) > 0) {
        if (c == '\n') break;
        if (len + 1 >= cap) {         // +1 para o '\0'
            cap *= 2;
            buf = realloc(buf, cap);
        }
        buf[len++] = c;
    }

    if (lidos <= 0 && len == 0) {     // EOF sem nenhum caractere lido
        free(buf);
        return NULL;
    }
    buf[len] = '\0';
    return buf;
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

    sem_t limite;
    sem_init(&limite, 0, max_threads);     // valor inicial = numero de vagas
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int cap = 2, n = 0;
    pthread_t *tids = malloc(sizeof(pthread_t) * cap);
    t_arg **args = malloc(sizeof(t_arg *) * cap);

    char *linha;
    while ((linha = read_line(fd)) != NULL) {
        if (n == cap) {
            cap *= 2;
            tids = realloc(tids, sizeof(pthread_t) * cap);
            args = realloc(args, sizeof(t_arg *) * cap);
        }
        t_arg *a = malloc(sizeof(t_arg));  // cada thread tem SEU argumento no heap
        a->id = n;
        a->linha = linha;
        a->soma = 0;
        a->limite = &limite;
        a->mutex = &mutex;
        args[n] = a;
        pthread_create(&tids[n], NULL, soma_thread, a);
        n++;
    }
    close(fd);

    for (int i = 0; i < n; i++) {
        pthread_join(tids[i], NULL);
        printf("LINHA %02d SOMA %ld\n", args[i]->id, args[i]->soma);
        free(args[i]->linha);
        free(args[i]);
    }
    printf("TOTAL: %ld\n", total_geral);

    free(args);
    free(tids);
    sem_destroy(&limite);
    pthread_mutex_destroy(&mutex);
    return 0;
}
