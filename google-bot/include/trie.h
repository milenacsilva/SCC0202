#ifndef _GENERIC_TRIE_H
#define _GENERIC_TRIE_H

#include "utils.h"

typedef struct trie *Trie;


/*
    Initializes a `Trie` instance. Callers are responsible for `free()`ing the resulted structure using `trie_delete()`
*/
Trie trie_init();

/*
    Searchs for an word in the trie.
*/
bool trie_insert_word(Trie trie, string word);

/*
    Inserts for an word in the trie.
*/
bool trie_search_word(Trie trie, string key);

/*
    Deletes an trie instance.
*/
void trie_delete(Trie *trie);

#endif