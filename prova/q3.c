// Prova Modelo SisHard - Questao 3 (2,5)
// Leia o enunciado em prova.html antes de comecar!
// Compile: gcc -Wall -g q3.c -o q3
//
// Para arquivos use APENAS open, read, write e close (nada de fopen/fprintf).
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Variavel GLOBAL com o progresso do "trabalho"
long contador = 0;

// Simula um trabalho infinito. NAO altere esta funcao.
void trabalha() {
    while (1) {
        contador++;
        if (contador % 50 == 0) {
            usleep(1000);
        }
    }
}

// Crie AQUI a funcao que salva o contador em estado.txt

// Crie AQUI a funcao que le estado.txt (se existir) e devolve o valor salvo

// Crie AQUI os handlers de SIGINT e SIGUSR1

int main() {
    // Exiba o PID deste processo

    // Se existir estado.txt, continue a contagem a partir do valor salvo

    // Registre AQUI os handlers

    trabalha();
    return 0;
}
