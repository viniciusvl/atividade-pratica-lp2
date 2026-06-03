#include <ctype.h>

// conta palavras (sequências de não-espaços) num buffer de 'tam' bytes
long contar_palavras(const char *texto, long tam) {
    long total = 0;
    int dentro = 0;
    for (long i = 0; i < tam; i++) {
        if (isspace((unsigned char)texto[i])) dentro = 0;
        else if (!dentro) { dentro = 1; total++; }
    }
    return total;
}
