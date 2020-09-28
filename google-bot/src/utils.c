#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "utils.h"

char* read_line(FILE* stream) {
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

// pattern = "[,\\/]";
int regex_compiler(regex_t *reg, char *pattern, int flag) {
    if (reg == NULL || pattern == NULL) {
        return ERROR;
    }

    if (regcomp(reg, pattern, 0) != 0) {
        perror("Erro ao compilar expressão regular\n");
        return ERROR;
    }

    return SUCCESS;
}

char *regex_matcher(regex_t *reg, char *line, regmatch_t *match, int *start) { 
    if (regexec(reg, line, 1, match, REG_NOTBOL) != REG_NOERROR) {
        *start = -1;
        return strdup(line);
    }

    *start += match->rm_eo;
    return strndup(line, match->rm_eo - 1);
}

char **get_data_row(char *line, char *pattern, int *amnt_values) {
    regex_t reg;
    regmatch_t match;
    char **values = NULL;

    if (regex_compiler(&reg, pattern, REG_EXTENDED) == ERROR) {
        exit(1);
    }
    
    int i = 0;
    int start = 0;
    do {
        values = (char **) realloc(values, sizeof(char *) * (i + 1));
        values[i] = regex_matcher(&reg, line + start, &match, &start);
        ++i;
    } while(start != -1);

    *amnt_values = i;
    regfree(&reg);

    return values;
}

FILE *open_file(char *filename, char *flag) {
    FILE *fp = fopen(filename, flag);

    if (fp == NULL) {
        perror("Erro ao abrir o arquivo\n");
        exit(1);
    }

    return fp;
}

int binary_search(int *arr, int key, int min, int max) {
    if (max < min) {
        return min;
    }
    
    int mid = min + (max - min)/2; // Evita interger under/overflow
    if (arr[mid] == key) {
        return mid;
    } 

    if (arr[mid] > key) {
        return binary_search(arr, key, min, mid - 1);
    } 
    
    return binary_search(arr, key, mid + 1, max); // Se a chave for maior 
}
