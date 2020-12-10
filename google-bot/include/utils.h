#ifndef UTILS_H
#define UTILS_H

#define READ_INPUT_ERROR -1
#define BUFFER_SIZE 4096

#define ERROR -1
#define SUCCESS 0

#define TRUE 1
#define FALSE 0

#define bool int
#define string char*

struct terminators {
    int amnt_terminators;
    int *terminators;
};

/* 
    Reads an numeric line from a file 

    @param stream input file pointer 
*/
#define readnum(file, ...) \
    _readnum(file, (struct terminators) { \
        .amnt_terminators=0,\
        __VA_ARGS__\
    })
int _readnum(FILE *stream, const struct terminators);

/* 
    Reads a string from a input file. 
    Stops reading when fgetc(stream) finds a '\\n' character or EOF is reached.
    
    @param stream input file pointer
*/
string readline(FILE *stream);

/* 
    Opens an file and deals with any error generated

    @param file_path path to the file
    @param flag opening flag 
*/
FILE* open_file(char *file_path, char *flag);

#define read_input_stream(file, ...) \
    _read_input_stream(file, (struct terminators) { \
        .amnt_terminators = 2,\
        __VA_ARGS__\
    })
string _read_input_stream(FILE *stream, const struct terminators);    
    
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
char* my_strndup(const char* str, int n);


#endif