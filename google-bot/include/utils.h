#ifndef UTILS_H
#define UTILS_H

/* Para notificar erros durante  execução */
#define boolean int
#define ERROR -1
#define SUCCESS 0

/* Parser patterns */
#define CSV_PATTERN "[,\\]"

/* 
    Reads a line from a input file. 
    Stops reading when fgetc(stream) finds a '\\n' character or EOF is reached.
    
    @param stream input file pointer
*/
char* read_line(FILE* stream);

/* 
    Parses the values of a line given a pattern.

    @param line input line containing values
    @param pattern regex pattern that separates the values
    @param amnt_values address to an `int` in order to store the amnt of values read by the parser 
*/

/*
	Implementation of strdup(3).
	Both strdup and strndup are only declared on string.h in POSIX Systems and not standardized in C99
	so a local implementation is necessary

	Takes a string and returns a copy of it allocated with malloc

	@param string pointer to the memory being copied
*/
char* my_strdup(const char* string);

/*
	Implementation of strndup(3).
	Both strdup and strndup are only declared on string.h in POSIX Systems and not standardized in C99
	so a local implementation is necessary

	Takes a string and returns a copy of it allocated with malloc with at most n+1 bytes

	@param string pointer to the memory being copied
	@param n maximum amount of bytes to be copied
*/
char* my_strndup(const char* string, int n) ;

char** parser_reader(char *line, char *pattern, int *amnt_values);

/*
    Frees the array of strings read by the parser

    @param values parser values to be freed
    @param amnt_values amount of values  
*/
void parser_free_values(char **values, int amnt_values);

/* 
    Opens an file and deals with any error generated

    @param file_path path to the file
    @param flag opening flag 
*/
FILE* open_file(char *filename, char *flag);

/* 
    Reads an numeric line from a file 

    @param stream input file pointer 
*/
int read_line_num(FILE *stream);

#endif
