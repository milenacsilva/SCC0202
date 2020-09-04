#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>


#define READLINE_BUFFER 4096

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

char **parserReader(char *line, char *pattern, int *amountOfValues) {
    regex_t reg;
    regmatch_t match;

    if (!pattern)
        pattern = "[,\\/]";

    if (regcomp(&reg, pattern, REG_EXTENDED) != 0) {
        perror("Erro ao compilar expressão regular\n");
        exit(1);
    }

    char **values = NULL;
    
    int i, start;
    i = start = 0;
    while (regexec(&reg, line + start, 1, &match, REG_NOTBOL) == REG_NOERROR) {
        values = (char **) realloc(values, sizeof(char *) * (i + 2)); 
        values[i] = strncat(calloc(match.rm_so + 1, sizeof(char)), line + start, match.rm_eo - 1);

        start += match.rm_eo;
        ++i;  
    }

    values[i] = strdup(line + start);
    
    regfree(&reg);

    *amountOfValues = i + 1;
    return values;
}

// void freeValues(char **values) {

// }

int main() {
    char *teste = readLine(stdin);
    int amountOfValues;

    char **values = parserReader(teste, NULL, &amountOfValues);
    for (int i = 0; i < amountOfValues; ++i) {
        printf("%s\n", values[i]);
        free(values[i]);
    }

    free(values);
    free(teste);
}