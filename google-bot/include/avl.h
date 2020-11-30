#ifndef _AVL_H
#define _AVL_H

#include "site.h"

#define MAX_AMNT_SITES 1000

typedef struct _avl_t avl_t;

typedef enum {
    PREORDER,
    INORDER,
    POSTORDER
} printing_order;

typedef enum {
    CSV,
    TERMINAL
} printing_format;

/*
    Initializes an binary search tree instance. 
    Callers are responsible for `free()`ing it 
    using `avl_delete()`
*/
avl_t *avl_init();

/*
    Inserts a new `site_t*` in the `avl_t*`

    @param avl tree to insert the node on
    @param site new site to be inserted
*/
bool avl_insert(avl_t *avl, site_t *site);

/*
    Searchs for a `key` in an `avl_t*` and returns 
    if has found it or not.

    @param avl tree to be searched on
    @param key key to be found
*/
bool avl_search(avl_t *avl, int key);

/*
    Prints an `avl_t*` instance

    @param avl tree to be printed
    @param order traversal printing order
    @param format printing format
    @param outfile pointer to outstream
*/
void avl_print(avl_t *avl, printing_order order, printing_format format, FILE *outfile);

/*
    Deletes an `avl_t*` instance.

    @param avl address to the tree
    @param delete_site if TRUE, deletes the `site_t*` in a node; if FALSE, does not
*/
void avl_delete(avl_t **avl, bool delete_site);

/*
    Deletes an `no_t*` containing a speficied
    `key` from a `avl_t*`


    @param avl address to an `avl_t*` instance
    @param key key of the `site_t` to be deleted 
*/
bool avl_delete_site(avl_t *avl, int key);

/*
    Returns the amount of sites in a `avl_t*`

    @param avl tree instance
*/
int avl_get_amnt_sites(avl_t *avl);

/*
    Searchs and returns the `site_t*`
    (if it exists) given its `key` 

    @param avl tree to be searched
    @param key key of the site to be searched
*/
site_t *avl_get_site(avl_t *avl, int key);

/*
    Searchs for an `keyword` in a tree and 
    returns a array of `site_t*` containing 
    it.

    @param avl tree to be searched
    @param keyword keyword to be searched
    @param amnt_matches address to an int storing the amount of matches
*/
site_t** avl_search_keyword(avl_t *avl, string keyword, int *amnt_matches);

/*
    Searchs for an `keyword` in a tree and 
    returns a array of `site_t*` containing 
    it.

    @param avl tree to be searched
    @param keyword keyword to be searched
    @param amnt_matches address to an int storing the amount of matches
*/
site_t** get_most_relevant_suggestions(avl_t *avl, string keyword, int max_amnt_suggestions);

#endif