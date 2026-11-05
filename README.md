# SisHard Resumo

Resumo completo de **Sistemas Hardware-Software (Insper, Ciência da Computação)**, montado a partir do
[site oficial da disciplina](https://insper.github.io/SistemasHardwareSoftwareBCC/), com uma **prova final modelo** para estudar.

## Conteúdo

| Página | Assunto |
| --- | --- |
| `index.html` | Visão geral, formato da Prova Final (AF), critério de aprovação, roteiro e checklist |
| `processos.html` | SO, kernel, POSIX, `fork`, `wait`/`waitpid` e macros, `argc`/`argv`, `execvp` |
| `entrada-saida.html` | `open`/`read`/`write`/`close`, flags, permissões, `read_line`, números em arquivo |
| `sinais.html` | Tabela de sinais, `kill`, `sigaction`, `SIG_DFL`, `sa_mask`, handlers aninhados |
| `threads.html` | pthreads, argumentos no heap, race condition, região crítica, mutex |
| `semaforos.html` | Semáforos POSIX, Rendez-vous, diagrama de dependências, barreira, limite de N, produtor-consumidor |
| `parte1-assembly.html` | 1ª metade: inteiros, memória, structs, x86-64, gdb e engenharia reversa |
| `cola.html` | Consulta rápida para a prova: includes, modelos de código e erros comuns |
| `prova.html` | **Prova modelo**: 4 questões práticas no estilo da AF (com rubrica e gabarito) e um quiz com correção automática |

A pasta `prova/` tem os arquivos-base (`q1.c` a `q4.c`, `instavel.c`, `numeros.txt`), e a pasta `prova/gabarito/` tem as soluções.
Todos os gabaritos foram compilados com `gcc -Wall` e testados: a Q4 também passa no `valgrind --leak-check=full`.

## Como usar

O site é estático (HTML, CSS e JS puros, sem build). Para usar:

- **Localmente:** abra o `index.html` no navegador.
- **Online:** ative o GitHub Pages em *Settings → Pages → Deploy from a branch*, escolhendo a branch e a pasta `/ (root)`.

Para treinar a prova:

```bash
cd prova
gcc -Wall instavel.c -o instavel
gcc -Wall -g q1.c -o q1 -pthread
```

---
Material de estudo não oficial, feito a partir do conteúdo público da disciplina. Em caso de divergência, valem o site e o Blackboard da disciplina.
