#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RUNS        6
#define AQUECIMENTO 1
#define MEDICOES    (RUNS - AQUECIMENTO)

extern void create_input(int word_count);
extern long contar_palavras(const char *texto, long tam);
extern char *ler_arquivo(const char *caminho, long *tamanho);
extern long contar_palavras_par(const char *texto, long tam, int num_threads);

static double medir_seq(const char *buf, long tam, long *resultado) {
    double tempos[MEDICOES];
    struct timespec t0, t1;
    for (int i = 0; i < RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &t0);
        *resultado = contar_palavras(buf, tam);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        if (i >= AQUECIMENTO)
            tempos[i - AQUECIMENTO] = (t1.tv_sec - t0.tv_sec)
                                    + (t1.tv_nsec - t0.tv_nsec) / 1e9;
    }
    double soma = 0;
    for (int i = 0; i < MEDICOES; i++) soma += tempos[i];
    return soma / MEDICOES;
}

static double medir_par(const char *buf, long tam, int nt,
                         long *resultado, double tempos_out[MEDICOES]) {
    struct timespec t0, t1;
    for (int i = 0; i < RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &t0);
        *resultado = contar_palavras_par(buf, tam, nt);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        if (i >= AQUECIMENTO)
            tempos_out[i - AQUECIMENTO] = (t1.tv_sec - t0.tv_sec)
                                        + (t1.tv_nsec - t0.tv_nsec) / 1e9;
    }
    double soma = 0;
    for (int i = 0; i < MEDICOES; i++) soma += tempos_out[i];
    return soma / MEDICOES;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <num_palavras>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_palavras = atoi(argv[1]);
    if (num_palavras < 1) {
        fprintf(stderr, "Numero de palavras invalido: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    create_input(num_palavras);

    long tamanho;
    char *buffer = ler_arquivo("inputs/words.txt", &tamanho);
    if (!buffer) return EXIT_FAILURE;

    // --- Teste 2: Corretude Sequencial ---
    long total_seq;
    double t_seq = medir_seq(buffer, tamanho, &total_seq);

    printf("\n=== Teste 2 — Corretude Sequencial ===\n");
    printf("Palavras (referencia) : %ld\n", total_seq);
    printf("T_seq (media %d runs) : %.6f s\n", MEDICOES, t_seq);

    // --- Testes 3, 4 e 6: Paralelo com T = 1, 2, 4, 8 ---
    int tcs[] = {1, 2, 4, 8};
    int n = 4;

    printf("\n=== Testes 3, 4 e 6 — Corretude Paralela e Speedup ===\n");
    printf("%-8s  %-12s  %-10s  %-12s  %-8s\n",
           "Threads", "Palavras", "Corretude", "T_par (s)", "Speedup");
    printf("%-8s  %-12s  %-10s  %-12s  %-8s\n",
           "-------", "--------", "---------", "---------", "-------");

    double t_par_t1 = 0;
    double tempos_t8[MEDICOES];
    double t_par_t8 = 0;

    for (int i = 0; i < n; i++) {
        int t = tcs[i];
        long total_par;
        double runs[MEDICOES];
        double t_par = medir_par(buffer, tamanho, t, &total_par, runs);
        double speedup = t_seq / t_par;
        const char *ok = (total_par == total_seq) ? "OK" : "FALHA";

        printf("%-8d  %-12ld  %-10s  %-12.6f  %.2fx\n",
               t, total_par, ok, t_par, speedup);

        if (t == 1) t_par_t1 = t_par;
        if (t == 8) {
            for (int j = 0; j < MEDICOES; j++) tempos_t8[j] = runs[j];
            t_par_t8 = t_par;
        }
    }

    // --- Teste 4: Sanidade T=1 ---
    printf("\n=== Teste 4 — Sanidade T=1 ===\n");
    double speedup_t1 = t_seq / t_par_t1;
    printf("Speedup (T=1) : %.2fx  %s\n",
           speedup_t1, speedup_t1 >= 0.8 ? "(OK — proximo de 1.0)" : "(ATENCAO — overhead elevado)");

    // --- Teste 5: Estabilidade (T=8) ---
    printf("\n=== Teste 5 — Estabilidade da Medicao (T=8) ===\n");
    double soma2 = 0;
    for (int i = 0; i < MEDICOES; i++) {
        printf("  Run %d : %.6f s\n", i + 1, tempos_t8[i]);
        soma2 += (tempos_t8[i] - t_par_t8) * (tempos_t8[i] - t_par_t8);
    }
    double desvio = sqrt(soma2 / MEDICOES);
    printf("  Media         : %.6f s\n", t_par_t8);
    printf("  Desvio padrao : %.6f s\n", desvio);
    printf("  Variancia rel : %.2f%%\n", (desvio / t_par_t8) * 100.0);

    free(buffer);
    return EXIT_SUCCESS;
}
