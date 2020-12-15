#ifndef _LIST_H
#define _LIST_H

#include "site.h"

typedef struct list *List;


/* 
    Initialites a `List` of `Site`s. Callers are responsible
    for `free()`ing it using `list_delete()`. 
*/
List list_init();

/* 
    Inserts an `Site` in the `List`. Callers cannot free the `Site` alone after inserting it. 
*/
bool list_insert_site(List s_list, Site site);

/* 
    Deletes an instance of `List`.
*/
void list_delete(List *s_list);

/* 
    Prints the `List` of `Site`s.
*/
void list_print(List s_list, int amnt_sites_to_print);

/* 
    Go to the next `Site` in the list starting at he `head`. Walks with the `cur` pointer.
*/
int sitelis_get_amnt_sites(List s_list);


void list_go_to_next_site(List s_list);

/* 
    Returns the `Site` in the `cur` pointer in the list.
*/
Site list_get_cur_site(List s_list);

#endif