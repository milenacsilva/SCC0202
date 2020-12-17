#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <regex.h>

#define bool int
#define READLINE_BUFFER 4096 // Buffer to maximize realloc's effeciency

#include "utils.h"

static int _read_input_stream_get_char(FILE *stream, const struct terminators t);
string _read_input_stream(FILE *stream, const struct terminators t);

/* 
    Opens an file and deals with any errors generated
*/
FILE* open_file(const char *file_path, const char *flag) {
    FILE *fp = fopen(file_path, flag);

    if (fp == NULL) { 
        perror("Error");
        exit(EXIT_FAILURE);
    }

    return fp;
}

/* 
    Reads an numeric line from a file 
*/
int _readnum(FILE *stream, struct terminators t) {
    char *tmp_num = _read_input_stream(stream,  t);
    int num = atoi(tmp_num);
    
    free(tmp_num);
    return num;
} 

/*
    Reads an string from a file
*/
string readline(FILE *stream) {
    string s1 = read_input_stream(stream, 
                                  .amnt_terminators=2,
                                   .terminators=(int[]){'\n', EOF});
    return s1;
}

static int _read_input_stream_get_char(FILE *stream, const struct terminators t) {
    int c = fgetc(stream);

    for (int i = 0; i < t.amnt_terminators; ++i) {
        if (c == '\r') {
            c = fgetc(stream); 
        } 
        if (c == t.terminators[i]) {
            return 0;
        } 
    }
  
    return c;
}



/*
    Reads an input stream until it reaches one of the passed termination chars
*/
string _read_input_stream(FILE *stream, const struct terminators t) {
    if (feof(stream)) {
        return NULL;
    }

    if (t.amnt_terminators == 0) {
        printf("Erro ao ler string: nenhum caracter de terminação\n");
        return NULL;
    }

    char *buffer = NULL;
    int buffer_len = 0;
    int c;

    do {
        if (buffer_len % BUFFER_SIZE == 0) {
            int j = (buffer_len / BUFFER_SIZE + 1) * BUFFER_SIZE;
            buffer = realloc(buffer, sizeof(char) * j);
        }
 
        c = _read_input_stream_get_char(stream, t);
        buffer[buffer_len] = c;
        buffer_len++;
    } while (c);

    if (buffer_len < 2) {
        free(buffer);
        return NULL;
    }
    if (buffer[buffer_len - 2] ==  ' ') {
        buffer[--buffer_len - 1] = '\0';
    }

    buffer = realloc(buffer, sizeof(char) * buffer_len);

    return buffer;
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
