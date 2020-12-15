#ifndef _KEYWORDS_H
#define _KEYWORDS_H

#include "utils.h"
typedef struct k_avl  *KeywordAvl;

/*
    Initializes an `KeywordAvl` instance. Callers are responsible
    for `free()`ing it using `avl_delete()`
*/
KeywordAvl keyword_avl_init();

/* 
    Inserts a new `Site` in the `KeywordAvl`. 
*/
bool keyword_avl_insert(KeywordAvl avl, string keyword);

/*
    Searchs for a `key` in an `KeywordAvl` and returns 
    if has found it or not.
*/
bool keyword_avl_search(KeywordAvl avl, string keyword);


/* 
    Deletes an `KeywordAvl` instance. 
*/
void keyword_avl_delete(KeywordAvl *avl);

#endif