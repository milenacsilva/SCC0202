#include<stdlib.h>
#include<stdio.h>

#define READLINE_BUFFER 4096

char* readLine(FILE* stream) {
    char *str = NULL;
    int i = 0;
    if (feof(stream))
    return NULL;

    do {
        if (i % READLINE_BUFFER == 0) {
            int j = (i / READLINE_BUFFER + 1) * READLINE_BUFFER;
            str = (char* ) realloc(str, sizeof(char ) * j);
        }
        str[i] = fgetc(stream);
        ++i;
    } while (str[i-1] != '\n' && str[i-1] != EOF);

    if (i <= 1)
        return NULL;   // No caso se ter um \n no final da última linha do input

    str = (char* ) realloc(str, sizeof(char ) * i); //Elimina o espaço adicional alocado
    str[i-1] = '\0'; //Torna em uma string

    return str;
}
