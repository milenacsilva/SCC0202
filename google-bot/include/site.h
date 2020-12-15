#ifndef SITE_H
#define SITE_H

/* Site especifications*/
#define MAX_KEY_DIGTS 4
#define MAX_RELEVANCY 1000
#define MIN_RELEVANCY 0
#define MAX_KEYWORDS 10
#define MAX_KEYWORD_LEN 50
#define MAX_NAME_LEN 50
#define MAX_LINK_LEN 100
#define MAX_AMNT_VALUES 14

#include "utils.h"

typedef enum { // Values indexes
    KEY,
    NAME,
    RELEVANCY,
    LINK,
    KEYWORDS
} DATA_COLUMNS;

typedef struct site* Site;

/* 
    Initializes a `Site` instance from a `values` arr. 
    Callers are responsible for `free()`ing it using
    `site_delete()`.    
*/
Site site_init(string *values, int amnt_keywords);

/*
    Deletes a `Site` instance given its address.
*/
void site_delete(Site *site);

/*
    Returns the key of an `Site` instance.
*/
int site_get_key(Site site);

/*
    Inserts a new `keyword` into a `Site` instance.
*/
bool site_insert_keyword(Site site, string keyword);

/*
    Prints a `Site` instance to an `outfile`.
*/
void site_print(Site site, FILE *f_out);

/*
    Updates the `relevancy` of a `Site` instance.
*/
bool site_update_relevancy(Site site, int relevancy);

/*
    Prints a `Site` instance in csv format.
*/
void site_print_in_csv_format(Site site, FILE *f_out);

/*
    Searches for a `keyword` in a `Site`'s keywords.
*/
string* site_get_keywords(Site site);

/*
    Returns the keywords array from a `Site`.
*/
bool site_search_keyword(Site site, string keyword);

/*
    Returns the amount of keywords a a `Site`.
*/
int site_get_amnt_keywords(Site site);

/*
    Returns the relevancy of a `Site`.
*/
int site_get_relevancy(Site site);

#endif