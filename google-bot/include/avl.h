#ifndef _AVL_H
#define _AVL_H

#include "site.h"
#include "list.h"


typedef struct avl  *Avl;

/* Traversal printing orders */
typedef enum {
    PREORDER,
    INORDER,
    POSTORDER
} PRINTING_ORDER;

/* Formating types for outfile */
typedef enum {
    CSV,
    TERMINAL
} PRINTING_FORMAT;

/*
    Initializes an `Avl` instance. Callers are responsible
    for `free()`ing it using `avl_delete()`
*/
Avl avl_init();

/* 
    Inserts a new `Site` in the `Avl`. 
*/
bool avl_insert(Avl avl, Site site);

/*
    Searchs for a `key` in an `Avl` and returns 
    if has found it or not.
*/
bool avl_search(Avl avl, int key);

/*
    Prints an `Avl` instance
*/
void avl_print(Avl avl, PRINTING_ORDER order, PRINTING_FORMAT format, FILE *outfile);

/*
    Deletes an `Avl` instance.
*/
void avl_delete(Avl *avl, bool delete_site);

/*
    Deletes an `Node*` containing a specified
    `key` from a `Avl`
*/
bool avl_delete_site(Avl avl, int key);

/*
    Returns the amount of sites in a `Avl`
*/
int avl_get_amnt_sites(Avl avl);

/*
    Searchs and returns the `Site`
    (if it exists) given its `key` 
*/
Site avl_get_site(Avl avl, int key);

/*
    Searchs for an `keyword` in a tree and 
    returns a array of `Site` containing it.
*/
List avl_search_keyword(Avl avl, string keyword);

/* 
    Get a suggestion of `Site`s based on a keyword`.
*/
List get_suggestions(Avl avl, string keyword, int max_amnt_suggestions);

#endif