#ifndef SITE_H
#define SITE_H

/* Site especifications */
#define MAX_KEY_DIGTS 4
#define MAX_RELEVANCY 1000
#define MIN_RELEVANCY 0
#define MAX_KEYWORDS 10
#define MAX_KEYWORD_LEN 50
#define MAX_NAME_LEN 50
#define MAX_LINK_LEN 100
#define MAX_AMNT_VALUES 14

#include "utils.h"

typedef enum _data_columns { // Values indexes
    KEY,
    NAME,
    RELEVANCY,
    LINK,
    KEYWORDS
} DATA_COLUMNS;

typedef struct _site SITE;

/*
    Initializes a site instance.

    @param values new site values
    @param amnt_values amount of values 
*/
SITE *site_init(char **values, int amnt_keywords);

/*
    Deletes an site instance.

    @param site address to an site instance
*/
void site_delete(SITE **site);

/*
    Returns the key of an site instance

    @param site site instance
*/
int site_get_key(SITE *site);

/*
    Inserts an new keyword into an site instance. Returns -1 in case of error and 0 if the operation is a success.

    @param site site instance
    @param keyword new keyword to be added
*/
boolean site_insert_keyword(SITE *site, char *keyword);

/*
    Prints the values of an site instance

    @param site site instance
    @param f_out output file pointer
*/
void site_print(SITE *site, FILE *f_out);

/*
    Updates the relevância of an site instance

    @param site site instance
    @param relevancy new site relevancy
*/
boolean site_update_relevancy(SITE *site, int relevancy);

/* 
    Prints the information about a site in csv format

    @param site instance of a site
    @param f_out output file pointer
*/
void site_print_in_csv_format(SITE *site, FILE *f_out);

#endif