#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "utils.h"
#include "parser.h"

#define ERROR -32000
struct _csvParser {
    FILE *file_pointer;
    HEADER header;
    READER reader;
    int amountOfValues; 
};

HEADER get_csvHeader();

//TO-DO: Make a function to manage file
CSVPARSER *csvParser_init(char *filename) {
    CSVPARSER *parser = malloc(sizeof(CSVPARSER));
    
    if (!parser) return NULL;

    parser->header = get_csvHeader();
    parser->reader = parserReader;

    return parser;
}

// Fazer uma struct para o reader
READER csvParser_get_reader() 

//TO-DO: Fazer uma função para lidar com o regex 
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

/* This should be at utils.h */
void freeValues(char **values, int amountOfValues) {
	for(int i = 0; i < amountOfValues; i++) {
		free(values[i]);
	}
	free(values);
}

int main() {
    char *teste = readLine(stdin);
    int amountOfValues;

    char **values = parserReader(teste, NULL, &amountOfValues);
    for (int i = 0; i < amountOfValues; ++i) {
        printf("%s\n", values[i]);
    }

	freeValues(values, amountOfValues);
	free(teste);
}
