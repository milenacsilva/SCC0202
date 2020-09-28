#ifndef SITES_LIST_H
#define SITES_LIST_H

#define MAX_AMNT_SITES 100

#include "site.h"

typedef struct slist SLIST;

SLIST *slist_init();
void slist_delete(SLIST **s_list);
boolean slist_insert_sorted_site(SLIST *s_list, SITE *site);
boolean slist_remove_site(SLIST *s_list, int key);
void slist_print(SLIST *s_list);
SITE *slist_get_site(SLIST *s_list, int key);


#endif