#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

long contar_palavras(const char *texto, long tam) {
    long total = 0;
    int dentro = 0;
    for (long i = 0; i < tam; i++) {
        if (isspace((unsigned char)texto[i])) dentro = 0;
        else if (!dentro) { dentro = 1; total++; }
    }
    return total;
}

typedef struct {
    const char *texto;
    long inicio;
    long fim;
    long parcial;
} Arg;

static void *worker(void *p) {
    Arg *arg = (Arg *)p;
    const char *texto = arg->texto;
    long inicio = arg->inicio;
    long fim = arg->fim;

    // Se o char anterior não é espaço, estamos no meio de uma palavra —
    // ela pertence à thread anterior, então iniciamos como "dentro".
    int dentro = (inicio > 0 && !isspace((unsigned char)texto[inicio - 1])) ? 1 : 0;

    long total = 0;
    for (long i = inicio; i < fim; i++) {
        if (isspace((unsigned char)texto[i])) dentro = 0;
        else if (!dentro) { dentro = 1; total++; }
    }

    arg->parcial = total;
    return NULL;
}

long contar_palavras_par(const char *texto, long tam, int num_threads) {
    pthread_t *tids = malloc(num_threads * sizeof(pthread_t));
    Arg *args       = malloc(num_threads * sizeof(Arg));
    if (!tids || !args) {
        perror("Erro ao alocar threads");
        exit(EXIT_FAILURE);
    }

    long fatia = tam / num_threads;

    for (int i = 0; i < num_threads; i++) {
        args[i].texto   = texto;
        args[i].inicio  = i * fatia;
        args[i].fim     = (i == num_threads - 1) ? tam : (i + 1) * fatia;
        args[i].parcial = 0;
        pthread_create(&tids[i], NULL, worker, &args[i]);
    }

    long total = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(tids[i], NULL);
        total += args[i].parcial;
    }

    free(tids);
    free(args);
    return total;
}
