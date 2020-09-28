#ifndef PARSER_H

    #define PARSER_H

    #include <stdio.h>

    #define ERROR -1
    #define SUCCESS 1


    typedef struct _csvparser CSVPARSER;
    typedef char ** (*READER)(char *, char *, int *);
    typedef struct _header HEADER;
    
    CSVPARSER *csvparser_init(FILE *fp);
    
    READER csvparser_get_reader();
    HEADER *csvparser_get_header();
    
    int csvparser_write(char **values);

    void csvparser_delete(CSVPARSER *parser);

#endif