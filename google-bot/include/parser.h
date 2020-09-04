#ifndef _PARSER_H
	
	#define _PARSER_H

	typedef char ** (*READER)(char *, char *, int *);


	char **parserReader(char *line, char *pattern, int *amountOfValues);
	void freeValues(char **values, int amountOfValues);

#endif
