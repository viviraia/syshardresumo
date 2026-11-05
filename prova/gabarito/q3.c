// GABARITO - Prova Modelo SisHard - Questao 3
// Compile: gcc -Wall -g q3.c -o q3
// Teste:   ./q3 &     kill -USR1 <pid>     kill -INT <pid>     cat estado.txt
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

long contador = 0;

void trabalha() {
    while (1) {
        contador++;
        if (contador % 50 == 0) {
            usleep(1000);
        }
    }
}

// Salva o contador em formato TEXTO usando so open/write/close
void salva_estado() {
    int fd = open("estado.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "%ld\n", contador); // numero -> texto
    write(fd, buf, n);
    close(fd);
}

// Le estado.txt (se existir). Retorna 0 se o arquivo nao existir.
long le_estado() {
    int fd = open("estado.txt", O_RDONLY);
    if (fd == -1) {
        return 0; // ainda nao existe: comeca do zero
    }
    char buf[64];
    int total = 0, lidos;
    // read pode devolver menos bytes que o pedido: le ate o EOF (retorno 0)
    while (total < (int)sizeof(buf) - 1 &&
           (lidos = read(fd, buf + total, sizeof(buf) - 1 - total)) > 0) {
        total += lidos;
    }
    buf[total] = '\0';
    close(fd);
    return atol(buf); // texto -> numero
}

void handler_usr1(int sig) {
    printf("Progresso: %ld\n", contador);
}

void handler_int(int sig) {
    salva_estado();
    printf("\nEstado salvo: %ld\n", contador);

    // Volta ao comportamento padrao e reenvia o sinal:
    // assim o processo termina DE VERDADE por SIGINT (o shell fica sabendo).
    struct sigaction padrao;
    padrao.sa_handler = SIG_DFL;
    padrao.sa_flags = 0;
    sigemptyset(&padrao.sa_mask);
    sigaction(SIGINT, &padrao, NULL);
    raise(SIGINT); // ou kill(getpid(), SIGINT);
}

int main() {
    printf("Meu PID: %d\n", getpid());

    contador = le_estado();
    printf("Comecando de %ld\n", contador);

    // SIGINT: durante o handler, bloqueia SIGTERM e SIGUSR1
    struct sigaction s_int;
    s_int.sa_handler = handler_int;
    s_int.sa_flags = 0;
    sigemptyset(&s_int.sa_mask);
    sigaddset(&s_int.sa_mask, SIGTERM);
    sigaddset(&s_int.sa_mask, SIGUSR1);
    sigaction(SIGINT, &s_int, NULL);

    // SIGUSR1: imprime o progresso
    struct sigaction s_usr1;
    s_usr1.sa_handler = handler_usr1;
    s_usr1.sa_flags = 0;
    sigemptyset(&s_usr1.sa_mask);
    sigaction(SIGUSR1, &s_usr1, NULL);

    trabalha();
    return 0;
}
