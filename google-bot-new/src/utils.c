#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "utils.h"

/* Função que lê uma linha de texto de um arquivo */
char* read_line(FILE* stream) {
    char *str = NULL;
    int i = 0;
    
    if (feof(stream))
        return NULL;

    do {
        if (i % READLINE_BUFFER == 0) {
            int j = (i / READLINE_BUFFER + 1) * READLINE_BUFFER;
            str = realloc(str, sizeof(char) * j);
        }
        str[i] = fgetc(stream);
        ++i;
    } while (str[i-1] != '\n' && str[i-1] != EOF);

    if (i <= 1)
        return NULL;   // No caso se ter um \n no final da última linha do input

    str = realloc(str, sizeof(char) * i); //Elimina o espaço adicional alocado
    str[i-1] = '\0'; //Torna em uma string

    return str;
}

/* Função que compila uma expressão regular e trata os erros */
int regex_compiler(regex_t *reg, char *pattern, int flag) {
    if (reg == NULL || pattern == NULL) {
        return ERROR;
    }

    if (regcomp(reg, pattern, 0) != 0) {
        return ERROR;
    }

    return SUCCESS;
}

/* Função que copia as palavra que dão match */ 
char *regex_matcher(regex_t *reg, char *line, regmatch_t *match, int *start) { 
    if (regexec(reg, line, 1, match, REG_NOTBOL) != REG_NOERROR) {
        *start = -1;
        return strdup(line);
    }

    *start += match->rm_eo;
    return strndup(line, match->rm_eo - 1);
}

/* Função que os valores de uma linha dado um padrão, no nosso caso, será
usado CSV (pattern = "[,\\/]") */
char **get_values(char *line, char *pattern, int *amnt_values) {
    regex_t reg;
    regmatch_t match;
    char **values = NULL;

    if (regex_compiler(&reg, pattern, REG_EXTENDED) == ERROR) {
        fprintf(stderr, "Erro ao compilar expressão regular\n");
        exit(EXIT_FAILURE);
    }
    
   *amnt_values = 0;
    int start = 0;

    do {
        values = realloc(values, sizeof(char *) * (*amnt_values + 1));
        values[*amnt_values] = regex_matcher(&reg, line + start, &match, &start);
        *amnt_values += 1;
    } while (start != -1);

    regfree(&reg);

    return values;
}

/* Função que da free em um vetor duplo de char contendo os valores */
void free_values(char **values, int amnt_values) {
    for (int i = 0; i < amnt_values; ++i) {
        free(values[i]);
        values[i] = NULL;
    }
    free(values);
    values = NULL;
}

/* Função que abre um arquivo e trata erros na sua abertura */
FILE *open_file(char *filename, char *flag) {
    FILE *fp = fopen(filename, flag);

    if (fp == NULL) {
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    return fp;
}

/* Função modificada de um binary search que age de maneira diferente
dependendo da flag passada */
int get_index(int *arr, int key, int min, int max, int flag) {
    if (max < min) {
        return flag == TO_FIND ? INVALID_INDEX : min;
    }
    
    int mid = min + (max - min)/2; // Evita interger under/overflow
    if (arr[mid] == key) {
        return flag == TO_INSERT ? INVALID_INDEX : mid;
    } 

    if (arr[mid] > key) {
        return get_index(arr, key, min, mid - 1, flag);
    } 
    
    return get_index(arr, key, mid + 1, max, flag); // Se a chave for maior 
}
