#ifndef _UTILS_H
#define _UTILS_H

#define READLINE_BUFFER 4096

#define TRUE 1
#define FALSE 0

#define boolean int
#define ERROR -1
#define SUCCESS 0

char* read_line(FILE* stream);
char **get_data_row(char *line, char *pattern, int *amnt_values);
FILE *open_file(char *filename, char *flag);
int binary_search(int *arr, int key, int min, int max);
#endif
