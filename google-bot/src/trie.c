#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "trie.h"
#include "utils.h"

#define AMNT_LETTERS 26
#define get_char_index(cur_char) (tolower(cur_char) - 'a')   

typedef struct node *Node;
struct node {
    bool is_leaf;
    Node *children;
};

struct trie {
    Node root;
};

/* Initializes an `Node` instance. */
static Node _trie_node_init() {
    Node new_node = malloc(sizeof(*new_node));
    assert(new_node != NULL);
    
    new_node->children = malloc(sizeof(*new_node->children)*AMNT_LETTERS);
    for (int i = 0; i < AMNT_LETTERS; ++i)  
        new_node->children[i] = NULL;
    
    new_node->is_leaf = FALSE;

    return new_node;
}

/* Searchs for a `word in the `Trie`. */
static Node _search(Node root, string word, int i) {
    if (root == NULL || (i == strlen(word) && !root->is_leaf)) return NULL; // If hasn't found the searched word
    if (i == strlen(word)) return root; // If has found

    if (!isalnum(word[i])) return NULL;

    int char_index = get_char_index(word[i]);
    return _search(root->children[char_index], word, i + 1);
}

/* Inserts an new `word` into the `Trie`. */
static void _insert(Node root, string word, int i) {
    if (i == strlen(word)) {
        root->is_leaf = TRUE;
        return;
    }

    if (!isalpha(word[i])) return;
    
    int char_index = get_char_index(word[i]);
    if (root->children[char_index] == NULL) //If suffix doesn't exists yet
        root->children[char_index] = _trie_node_init();

    return _insert(root->children[char_index], word, i + 1);
}

/* Recursively deletes all `Node`s from `Trie`. */
static void _delete_nodes(Node *root) {
    if (*root == NULL) return;

    for (int i = 0; i < AMNT_LETTERS; ++i)
        _delete_nodes(&(*root)->children[i]);
    
    free((*root)->children);
    (*root)->children = NULL;

    free(*root);
}    

/* Initializes a `Trie` instance. */
Trie trie_init() {
    Trie trie = malloc(sizeof(*trie));
    assert(trie != NULL);

    if ((trie->root = _trie_node_init()) == NULL) return NULL;
    return trie;    
}

/* Searchs for an word in the trie. */
bool trie_search_word(Trie trie, string word) {
    assert(trie != NULL);
    assert(trie->root != NULL);

    return _search(trie->root, word, 0) == NULL ? FALSE : TRUE;
}

/* Inserts for an word in the trie. */
bool trie_insert_word(Trie trie, string word) {
    assert(trie != NULL);
    assert(trie->root != NULL);

    _insert(trie->root, word, 0);
 
    return SUCCESS;
}

/* Deletes an trie instance. */
void trie_delete(Trie *trie) {
    if (*trie == NULL) return;
    
    _delete_nodes(&(*trie)->root);
    free(*trie);
    *trie = NULL;
}

