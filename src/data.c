#include <stdio.h>
#include <stdlib.h>

char *ler_arquivo(const char *caminho, long *tamanho) {
    FILE *file = fopen(caminho, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *tamanho = ftell(file);
    rewind(file);

    char *buffer = malloc(*tamanho);
    if (!buffer) {
        perror("Erro ao alocar memoria");
        fclose(file);
        return NULL;
    }

    if (fread(buffer, 1, *tamanho, file) != (size_t)*tamanho) {
        perror("Erro ao ler arquivo");
        fclose(file);
        free(buffer);
        return NULL;
    }
    fclose(file);
    return buffer;
}
