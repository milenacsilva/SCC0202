#ifndef _PARSER_H
	
	#define _PARSER_H

	char **parserReader(char *line, char *pattern, int *amountOfValues);
	void freeValues(char **values, int amountOfValues);

#endif
