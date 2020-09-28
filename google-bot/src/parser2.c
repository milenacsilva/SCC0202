#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "parser2.h"
#include "utils.h"

struct _header {
    char **fields;
    int amnt_fields;
};

struct _csvparser {
    FILE *file_pointer;
    HEADER *header;
    READER reader;
};

//TO-DO: Fazer uma função para lidar com o regex 
char **get_values(char *line, char *pattern) {
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

    return values;
}

HEADER *init_header(FILE *fp) {
    HEADER *header = malloc(sizeof(HEADER));
    
    if (header != NULL) {
        char *line = readLine(fp);
    
        header->amnt_fields = 0;
        for (int i = 0; line[i]; ++i) ++header->amnt_fields;
        ++header->amnt_fields;

        header->fields = get_values(line, NULL);

        free(line);
    }
    
    return header;
}

HEADER *csvparser_get_header() {
    
}

CSVPARSER *csvparser_init(FILE *fp) {
    CSVPARSER *parser = malloc(sizeof(CSVPARSER));

    if (parser != NULL) {
        parser->file_pointer = fp;
        parser->header = init_header(fp);
        parser->reader = get_values;
    }
}
