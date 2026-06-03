#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_WORD_COUNT 10000
#define WORDS_PER_LINE 5
#define OUTPUT_PATH "inputs/words.txt"

void create_input(int word_count) {
    const char *words[] = {
        "threads", "concorrencia", "ufpb", "pedra", "chama",
        "professor", "bidu", "ponte", "sombra", "computador"
    };

    srand(time(NULL));

    FILE *file = fopen(OUTPUT_PATH, "w");
    if (!file) {
        perror("Erro ao abrir arquivo de saida");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < word_count; i++) {
        fprintf(file, "%s", words[rand() % 10]);

        if ((i + 1) % WORDS_PER_LINE == 0) {
            fprintf(file, "\n");
        } else {
            fprintf(file, " ");
        }
    }

    fclose(file);
}

