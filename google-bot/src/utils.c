/*-------------------------------------------------------------*
 |  /\_/\ 			  Trabalho feito por                /\_/\  |
 | ( o.o )	    Milena Corrêa da Silva - 11795401      ( o.o ) |
 |  > ^ <    Lourenço de Salles Roselino - 11796805     > ^ <  |
 *-------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "utils.h"

static int regex_compiler(regex_t *reg, char *pattern, int flags);
static char* regex_matcher(regex_t *reg, char *line, regmatch_t *match, int *start); 

/* 
    Reads a line from a input file. 
    Stops reading when fgetc(stream) finds a '\\n' character or EOF is reached.
*/
char* read_line(FILE* stream) {
    if (feof(stream)) // In case the pointer is already at the end of file
        return NULL;    
    
    char *str = NULL;
    int i = 0;
    
    do {
        if (i % BUFFER_SIZE == 0) {
            int j = (i / BUFFER_SIZE + 1) * BUFFER_SIZE;
            str = realloc(str, sizeof(char) * j);
        }
        str[i] = fgetc(stream);
        ++i;
    } while (str[i-1] != '\n' && str[i-1] != EOF);

	if(i<2) {
		free(str);
		return NULL;
	}

    str = realloc(str, sizeof(char) * i); // Removes extra allocated space

    str[i-1] = '\0'; 
    return str;
}

/*
	Implementation of strdup(3).
	Both strdup and strndup are only declared on str.h in POSIX Systems and not standardized in C99
	so a local implementation is necessary

	Takes a str and returns a copy of it allocated with malloc
*/
char* my_strdup(const char* str) {
	int size = strlen(str)+1;
	char* result = malloc(size);

	if(result != NULL) {
		memcpy(result, str, size);
	}
	return result;
}

/*
	Implementation of strndup(3).
	Both strdup and strndup are only declared on str.h in POSIX Systems and not standardized in C99
	so a local implementation is necessary

	Takes a str and returns a copy of it allocated with malloc with at most n+1 bytes
*/
char* my_strndup(const char* str, int n) {
	int size = strlen(str)+1;
	size = n < size ? n+1 : size;
	char* result = malloc(size);

	if(result != NULL) {
		memcpy(result, str, size);
		result[size-1] = '\0';
	}
	return result;
}

/* 
    Compiles a regular expression with a espeficied pattern and flag. 
    Returns -1 in case of error and 0 in case of SUCCESS.
*/
static int regex_compiler(regex_t *reg, char *pattern, int flags) {
    if (reg == NULL || pattern == NULL) { // If invalid arguments are passed
        return ERROR;
    }

    if (regcomp(reg, pattern, flags) != 0) {
        return ERROR;
    }

    return SUCCESS;
}

/* 
    Copies and returns blocks that match the regex pattern.
*/ 
static char* regex_matcher(regex_t *reg, char *line, regmatch_t *match, int *start) { 
    if (regexec(reg, line, 1, match, REG_NOTBOL) != REG_NOERROR) { // Copies the last block
        *start = -1;
        return my_strdup(line);
    }

    *start += match->rm_eo;
    return my_strndup(line, match->rm_eo - 1);
}

/* 
    Parses the values of a line given a pattern.
*/
char** parser_reader(char *line, char *pattern, int *amnt_values) {
    regex_t reg;
    regmatch_t match;
    char **values = NULL;

    if (regex_compiler(&reg, pattern, REG_EXTENDED) == ERROR) {
        printf("Erro ao compilar expressão regular\n");
        exit(EXIT_FAILURE);
    }
    
    *amnt_values = 0;
    int start = 0;
    do { // Copies values that match
        values = realloc(values, sizeof(char *) * (*amnt_values + 1));
        values[*amnt_values] = regex_matcher(&reg, line + start, &match, &start);
        *amnt_values += 1;
    } while (start != -1);

    regfree(&reg); 
    return values;
}

/*
    Frees the array of strings read by the parser

    @param values parser values to be freed
    @param amnt_values amount of values  
*/
void parser_free_values(char **values, int amnt_values) {
    for (int i = 0; i < amnt_values; ++i) {
        free(values[i]);
        values[i] = NULL;
    }
    free(values);
    values = NULL;
}

/* 
    Opens an file and deals with any errors generated
*/
FILE* open_file(char *file_path, char *flag) {
    FILE *fp = fopen(file_path, flag);

    if (fp == NULL) { 
        perror("Error: ");
        exit(EXIT_FAILURE);
    }

    return fp;
}

/* 
    Reads an numeric line from a file 
*/
int read_line_num(FILE *stream) {
    char *tmp_num = read_line(stream);
    int num = atoi(tmp_num);
    
    free(tmp_num);
    return num;
} 
