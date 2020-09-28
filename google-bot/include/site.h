#ifndef SITE_H
#define SITE_H

#define MAX_KEY_DIGTS 4
#define MAX_RELEVANCY 1000
#define MIN_RELEVANCY 0
#define MAX_KEYWORDS 10
#define MAX_KEYWORD_LEN 50
#define MAX_NAME_LEN 50
#define MAX_LINK_LEN 100

#include "utils.h"

typedef struct _site SITE;

SITE *site_init(char **data_row, int amnt_keywords);
void site_delete(SITE **site);
    
void site_print(SITE *site);
    
int site_get_key(SITE *site);
boolean site_insert_keyword(SITE *site, char *keyword);
boolean site_update_relevancy(SITE *site, int relevancy);

#endif