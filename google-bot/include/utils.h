#ifndef _UTILS_H
#define _UTILS_H

#define READLINE_BUFFER 4096

#define INVALID_INDEX -1 

#define boolean int
#define ERROR -1
#define SUCCESS 0

char* read_line(FILE* stream);
char **get_values(char *line, char *pattern, int *amnt_values);
void free_values(char **values, int amnt_values);
FILE *open_file(char *filename, char *flag);
int read_num(FILE *stream);

#endif
