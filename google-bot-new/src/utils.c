#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "utils.h"

/* Função que lê uma linha de texto de um arquivo */
char* read_line(FILE* stream) {
    char *str = NULL;
    int i = 0;
    
    if (feof(stream)) // Caso cehgue no fim do arquivo
        return NULL;

    do {
        if (i % READLINE_BUFFER == 0) {
            int j = (i / READLINE_BUFFER + 1) * READLINE_BUFFER;
            str = realloc(str, sizeof(char) * j);
        }
        str[i] = fgetc(stream);
        ++i;
    } while (str[i-1] != '\n' && str[i-1] != EOF);

    if (i <= 1) { // No caso se ter um \n no final da última linha do input
        free(str);
        return NULL;   
    }

    str = realloc(str, sizeof(char) * i); //Elimina o espaço adicional alocado
    str[i-1] = '\0'; //Torna em uma string

    return str;
}

/* Função que compila uma expressão regular e trata os erros */
int regex_compiler(regex_t *reg, char *pattern, int flag) {
    if (reg == NULL || pattern == NULL) { // Caso os objetos não tenham sido inicializados
        return ERROR;
    }

    if (regcomp(reg, pattern, 0) != 0) { // Caso não consiga compilar
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

    if (regex_compiler(&reg, pattern, REG_EXTENDED) == ERROR) { // Se não conseguir compilar
        printf("Erro ao compilar expressão regular\n");
        exit(EXIT_FAILURE);
    }
    
   *amnt_values = 0;
    int start = 0;

    do { // Copia os valores que deram match
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

    if (fp == NULL) { // Caso tenha algum erro ao abrir o arquivo
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    return fp;
}

/* Função equivalente a readline para números */
int read_num(FILE *stream) {
    char *tmp_num = read_line(stream);
    int num = atoi(tmp_num);
    
    free(tmp_num);

    return num;
} 
