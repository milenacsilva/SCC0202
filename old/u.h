#ifndef UTILS_H
#define UTILS_H

/* Para notificar erros durante  execução */
#define bool int
#define bool int
#define string char*


#define BUFFER_SIZE 4096 // Buffer to maximize realloc's effeciency

#define ERROR -1
#define SUCCESS 0

#define FOUND 1
#define NOT_FOUND 0

#define TRUE 1
#define FALSE 0

/* Parser patterns */
#define CSV_PATTERN "[,\\]"

/* 
    Reads a line from a input file. 
    Stops reading when fgetc(stream) finds a '\\n' character or EOF is reached.
    
    @param stream input file pointer
*/
char* read_line(FILE* stream);

/*
	Implementation of strdup(3).
	Both strdup and strndup are only declared on string.h in POSIX Systems and not standardized in C99
	so a local implementation is necessary

	Takes a string and returns a copy of it allocated with malloc

	@param string pointer to the memory being copied
*/
char* my_strdup(const char* str);

/*
	Implementation of strndup(3).
	Both strdup and strndup are only declared on string.h in POSIX Systems and not standardized in C99
	so a local implementation is necessary

	Takes a string and returns a copy of it allocated with malloc with at most n+1 bytes

	@param string pointer to the memory being copied
	@param n maximum amount of bytes to be copied
*/
char* my_strndup(const char* str, int n) ;

/* 
    Parses the values of a line given a pattern.

    @param line input line containing values
    @param pattern regex pattern that separates the values
    @param amnt_values address to an `int` in order to store the amnt of values read by the parser 
*/
char** parser_reader(string line, string pattern, int *amnt_values);

/*
    Frees the array of strings read by the parser

    @param values parser values to be freed
    @param amnt_values amount of values  
*/
void parser_free_values(string *values, int amnt_values);

/* 
    Opens an file and deals with any error generated

    @param file_path path to the file
    @param flag opening flag 
*/
FILE* open_file(string filename, string flag);

/* 
    Reads an numeric line from a file 

    @param stream input file pointer 
*/
int read_line_num(FILE *stream);

#endif
