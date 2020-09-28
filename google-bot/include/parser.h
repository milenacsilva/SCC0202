#ifndef _PARSER_H
	
	#define _PARSER_H

	#define HEADER char **

	typedef char ** (*READER)(char *, char *, int *);
	typedef struct _csvParser CSVPARSER; 

	CSVPARSER *csvParser_init(char *filename);
	void freeValues(char **values, int amountOfValues);

#endif
