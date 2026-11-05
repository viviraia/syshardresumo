// GABARITO - Prova Modelo SisHard - Questao 2
// Compile: gcc -Wall -g q2.c -o q2
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

        pid_t filho = fork();
        if (filho == 0) {
            // FILHO: vira o programa instavel.
            // argv sempre comeca com o nome do programa e termina com NULL
            char *args[] = {"./instavel", argv[1], argv[2], NULL};
            execvp(args[0], args);
            // so chega aqui se o exec FALHAR
            perror("execvp");
            exit(127);
        }

        // PAI
        int wst;
        waitpid(filho, &wst, 0);

        if (WIFEXITED(wst)) {
            // WEXITSTATUS devolve so 8 bits (0..255). "return -1" vira 255.
            // O cast para (char) recupera o valor com sinal (-1).
            int ret = (char) WEXITSTATUS(wst);
            printf("Tentativa %d: instavel retornou %d\n", tentativa, ret);

            if (ret == 0) {
                printf("SUCESSO apos %d tentativa(s)\n", tentativa);
                continuar = 0;
            } else if (ret < 0) {
                printf("ERRO: parametros invalidos, desistindo\n");
                continuar = 0;
            }
            // ret entre 1 e 3: falha temporaria -> tenta de novo
        } else if (WIFSIGNALED(wst)) {
            int sig = WTERMSIG(wst);
            printf("Tentativa %d: instavel terminou com o sinal %d (%s)\n",
                   tentativa, sig, strsignal(sig));
            // terminou de forma anormal -> tenta de novo
        }
    }

    return 0;
}
