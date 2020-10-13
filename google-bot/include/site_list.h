#ifndef SITE_LIST_H
#define SITE_LIST_H

#define MAX_AMNT_SITES 100
#define IS_IN_LIST 1
#define NOT_IN_LIST 0

#define TRUE 1
#define FALSE 0

#include "site.h"

typedef struct sitelist SITELIST;

/*
    Initialites an list of sites
*/
SITELIST *sitelist_init();

/*
    Deletes an instance of a site

    @param s_list adress to a site list instance
*/
void sitelist_delete(SITELIST **s_list);

/*
    Inserts an site in the list. Returns -1 for error and 0 for success.
    Callers cannot free the site instansce alone after inserting it.

    @param s_list instance of a site list
    @param site new site to be inserted
*/
boolean sitelist_insert_site(SITELIST *s_list, SITE *site);

/*
    Removes an instance os an site from de list. Returns -1 for error and 0 for success.

    @param s_list instance of a site list
    @param key key of the site to remove
*/
boolean sitelist_remove_site(SITELIST *s_list, int key);

/* 
    Returns an site instance for the site. 
    Callers cannot free the return value by theirselves.

    @param s_list instance of a site list
    @param key key for searched site
*/
SITE *sitelist_get_site(SITELIST *s_list, int key);

/*
    Prints the list of sites.
    
    @param s_list instance of a site to be printed
*/
void sitelist_print(SITELIST *s_list);

/* 
    Verifies if a site is full. Returs -1 for erros, 1 for true and 0 for false.

    @param s_list site list instance
*/
int sitelist_is_full(SITELIST *s_list);

/*
    Verifies if a site list is empty. Returs -1 for erros, 1 for true and 0 for false.

    @param s_list site list instance
*/
int sitelist_is_empty(SITELIST *s_list);

/*
    Writes de list of file to an csv formatted file 
    
    @param s_list instance of site list
    @param f_out output file pointer
*/
void sitelist_write_to_csv_file(SITELIST *s_list, FILE *f_out);

#endif