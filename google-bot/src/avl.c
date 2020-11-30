#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avl.h"
#include "site.h"
#include "utils.h"

#define height(node) (node == NULL ? 0 : node->height)
#define left_left(node, balance, key) (balance > 1 && key < site_get_key(node->left->site))
#define left_right(node, balance, key) (balance > 1 && key > site_get_key(node->left->site))
#define right_right(node, balance, key) (balance < -1 && key > site_get_key(node->right->site))
#define right_left(node, balance, key) (balance < -1 && key < site_get_key(node->right->site))


/* Funções de mínimo e máximo */
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)


typedef struct _no_t no_t;
struct _no_t {
    site_t *site;
    no_t *left;
    no_t *right;
    int height;
};

struct _avl_t {
    no_t *root;
    int amnt_sites;
};

static void _nullify_node(no_t **node, bool delete_site);
static no_t* _init_node(site_t *site);
static no_t* _search_node(no_t *root, int key);
static void _rotate_left(no_t **root);
static void _rotate_right(no_t **root);
static int _get_balance_factor(no_t *node);
static void _balance_tree(no_t **root, int key);
static bool _insert_node(no_t **root, site_t *site);
static void _print_inorder(no_t* root, printing_format format, FILE *outfile) ;
static void _print_preorder(no_t* root, printing_format format, FILE *outfile) ;
static void _print_postorder(no_t* root, printing_format format, FILE *outfile) ;
static void _delete_all_nodes(no_t **root, bool delete_site);
static no_t* _get_max_value(no_t *root);
static void _rebalance_tree(no_t **root);
static bool _delete_node(no_t **root, int key);


/*
    Frees and nullifies a `no_t*` instance
*/
static void _nullify_node(no_t **node, bool delete_site) {
    if (*node == NULL) return;
    
    if (delete_site == TRUE) site_delete(&(*node)->site);
    
    free(*node);
    *node = NULL;
}

/*
    Initilizes a `no_t*` instance. Callers are 
    responsible for `free()`ing it. 
*/
static no_t* _init_node(site_t *site) {
    no_t *new_node = malloc(sizeof(no_t));
    
    if (new_node == NULL) return NULL;

    new_node->left = new_node->right = NULL;
    new_node->site = site;
    new_node->height = 1;

    return new_node;
}

/*
    Searchs for a `no_t*` instance containing an
    specific `key` in an `avl_t*`. Has an `O(logn)` 
    time complexity.
*/
static no_t* _search_node(no_t *root, int key) {
    if (root == NULL) return NULL;
    
    if (site_get_key(root->site) == key) return root;

    if (site_get_key(root->site) > key) return _search_node(root->left, key);

    return _search_node(root->right, key);
}

/*
    Rotates an `no_t*` instance to te left
*/
static void _rotate_left(no_t **root) {
    no_t *new_root = (*root)->right;
    no_t *subtree = new_root->left;

    (*root)->right = subtree;
    new_root->left = *root;

    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));

    *root = new_root;
}

/*
    Rotates an `no_t*` instance to te right
*/
static void _rotate_right(no_t **root) {
    no_t *new_root = (*root)->left;
    no_t *subtree = new_root->right;

    (*root)->left = subtree;
    new_root->right = *root;

    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));

    *root = new_root;
}

/*
    Gets the balance factor, meaning the difference
    in heights from the left and right of a `no_t*` 
*/
static int _get_balance_factor(no_t *node) {
    return node == NULL ? 0 : height(node->left) - height(node->right);
}

/*
    Self-balances an `avl_t*` during the insertion
    of a new `no_t*`
*/
static void _balance_tree(no_t **root, int key) {
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    int balance = _get_balance_factor(*root);

    if (left_left((*root), balance, key))
        return _rotate_right(root); 

    if (left_right((*root), balance, key)) {
        _rotate_left(&(*root)->left);
        return _rotate_right(root);
    }

    if (right_right((*root), balance, key)) 
        return _rotate_left(root);

    if (right_left((*root), balance, key)) {
        _rotate_right(&(*root)->right);
        return _rotate_left(root);
    }
} 

/*
    Inserts a new `no_t*` in a `avl_t*` if its key
    doens't already exists. 
*/
static bool _insert_node(no_t **root, site_t *site) {
    if (*root == NULL) {
        *root = _init_node(site);
        return SUCCESS;
    }

    if (site_get_key((*root)->site) == site_get_key(site)) return ERROR;

    int return_flag;
    if (site_get_key((*root)->site) > site_get_key(site)) return_flag = _insert_node(&(*root)->left, site);
    else if (site_get_key((*root)->site) < site_get_key(site)) return_flag = _insert_node(&(*root)->right, site);

    _balance_tree(root, site_get_key(site));
    
    return return_flag;
}

/* 
    Prints an`avl_t*` in inorder
*/
static void _print_inorder(no_t *root, printing_format format, FILE *outfile) { 
    if (root == NULL) return;
    
    _print_inorder(root->left, format, outfile); 

    if (format == TERMINAL) site_print(root->site, outfile);
    else site_print_in_csv_format(root->site, outfile);  
    
    _print_inorder(root->right, format,outfile); 
} 

/* 
    Prints an`avl_t*` in preorder
*/
static void _print_preorder(no_t *root, printing_format format, FILE *outfile) { 
    if (root == NULL) return; 
    
    if (format == TERMINAL) site_print(root->site, outfile);
    else site_print_in_csv_format(root->site, outfile);  
    
    _print_preorder(root->left, format, outfile); 
    _print_preorder(root->right, format, outfile); 
} 

/* 
    Prints an`avl_t*` in postorder
*/
static void _print_postorder(no_t *root, printing_format format, FILE *outfile) { 
    if (root == NULL) return; 

    _print_postorder(root->left, format, outfile); 
    _print_postorder(root->right, format,outfile); 
    
    if (format == TERMINAL) site_print(root->site, outfile);
    else site_print_in_csv_format(root->site, outfile); 
} 

/*
    Deletes all `no_t*`'s recursively from a `avl_t*`
*/
static void _delete_all_nodes(no_t **root, bool delete_site) {
    if (*root == NULL) return;

    _delete_all_nodes(&(*root)->left, delete_site);
    _delete_all_nodes(&(*root)->right, delete_site);

    _nullify_node(root, delete_site);
}

/*
    Gets the max key value of a tree starting 
    at its root
*/
static no_t* _get_max_value(no_t *root) {
    if (root == NULL) return NULL;
    
    if (root->right == NULL) return root;

    return _get_max_value(root->right);
}

/*
    Self-rebalances an `avl_t*` after deleting 
    a `no_t*`
*/
static void _rebalance_tree(no_t **root) {
    (*root)->height = max(height((*root)->left), height((*root)->right)) + 1; 
    int balance = _get_balance_factor(*root); 

    if (balance > 1 && _get_balance_factor((*root)->left) >= 0) 
        _rotate_right(root); 
    if (balance > 1 && _get_balance_factor((*root)->left) < 0) { 
        _rotate_left(&(*root)->left); 
        return _rotate_right(root); 
    } 
    if (balance < -1 && _get_balance_factor((*root)->right) <= 0) 
        return  _rotate_left(root); 
    if (balance < -1 && _get_balance_factor((*root)->right) > 0) { 
        _rotate_right(&(*root)->right); 
        return  _rotate_left(root); 
    } 
}

/*
    Searchs and deletes a `no_t*` given its `key` 
*/
static bool _delete_node(no_t **root, int key) {
    if (*root == NULL) return ERROR;
    
    if (site_get_key((*root)->site) > key)  _delete_node(&(*root)->left, key);
    else if (site_get_key((*root)->site) < key) _delete_node(&(*root)->right, key);
    else {
        no_t *successor;
        if ((*root)->left == NULL) {
            successor = (*root)->right;
            
            site_delete(&(*root)->site);
            free(*root);
            *root = successor;    
            return SUCCESS;
        }
        
        if ((*root)->right == NULL) {
            successor = (*root)->left;

            site_delete(&(*root)->site);
            free(*root);
            *root = successor;
            return SUCCESS;
        }

        successor = _get_max_value((*root)->left);
        site_t *tmp = (*root)->site;
        (*root)->site = successor->site;
        (*root)->left = successor->left;
        
        successor->site = tmp;
        _nullify_node(&successor, TRUE);
    }

    if (*root == NULL) return SUCCESS;

    _rebalance_tree(root);
    return SUCCESS;
}

/*
    
*/
static void _get_amnt_matches(no_t *root, string keyword, int *amnt_matches) {
    if (root == NULL) return;   

    if (site_search_keyword(root->site, keyword) == FOUND) *amnt_matches += 1;

    _get_amnt_matches(root->left, keyword, amnt_matches);
    _get_amnt_matches(root->right, keyword, amnt_matches);
}

/*
    Matches the all the `site_t*` in an `avl_t*` 
    containing a keyword.
*/
static void _match_sites_with_keyword(no_t *root, string keyword, site_t **match_sites, int amnt_matches, int *i) {
    if (root == NULL) return;   
    
    if (*i == amnt_matches) return;

    if (site_search_keyword(root->site, keyword) == FOUND) { // If found a match       
        match_sites[*i] = root->site;
        *i += 1;
    }
    _match_sites_with_keyword(root->left, keyword, match_sites, amnt_matches, i);
    _match_sites_with_keyword(root->right, keyword, match_sites, amnt_matches, i);

}

/*
    Inserts a new `site_t*` sugestion if its not yet 
    in list or it's a higher relevancy than the ones 
    currently in it. 
*/
static void _insert_new_sugestion(site_t **most_relevant_suggestions, site_t *new_sugestion, int max_amnt_suggestions) {
    for (int i = 0; i < max_amnt_suggestions; ++i) {
        if (new_sugestion == most_relevant_suggestions[i]) return;
        
        if (site_get_relevancy(new_sugestion) > site_get_relevancy(most_relevant_suggestions[i])) {            
            most_relevant_suggestions[i] = new_sugestion;
            for (int j = i + 1; j < max_amnt_suggestions - 1; ++j) most_relevant_suggestions[j] = most_relevant_suggestions[j+1];
            
            return;
        }
    }
}

/*
    Gets the list of the most relevant `site_t*` 
    suggestion based on a keyword
*/
static site_t** _get_most_relevant_suggestions(avl_t *base_tree, string base_keyword, int max_amnt_suggestions) {  
    site_t **most_relevant_suggestions = calloc(max_amnt_suggestions, sizeof(site_t*));

    int amnt_base_matches;
    site_t **matches_with_base_keyword =  avl_search_keyword(base_tree, base_keyword, &amnt_base_matches); // n

    for (int i = 0; i < amnt_base_matches; ++i) { // n
        int amnt_keywords = site_get_amnt_keywords(matches_with_base_keyword[i]);
        string *keywords = site_get_keywords(matches_with_base_keyword[i]);
     
        for (int j = 0; j < amnt_keywords; ++j) { // 10
            int amnt_suggestions;
            site_t **suggestions = avl_search_keyword(base_tree, keywords[j], &amnt_suggestions); // n
            for (int k = 0; k < amnt_suggestions; k++) { // n 
                _insert_new_sugestion(most_relevant_suggestions, suggestions[k], max_amnt_suggestions);
            }
        
            free(suggestions);
            suggestions = NULL;
        }
    }

    return most_relevant_suggestions;
}

/*
    Initializes an binary search tree instance. 
    Callers are responsible for `free()`ing it 
    using `avl_delete()`
*/
avl_t *avl_init() {
    avl_t *avl = malloc(sizeof(avl_t));
  
    assert(avl != NULL); // In case cannot initialize the instance

    avl->root = NULL;
    avl->amnt_sites = 0;

    return avl;
}

/*
    Inserts a new `site_t*` in the `avl_t*`

    @param avl tree to insert the node on
    @param site new site to be inserted
*/
bool avl_insert(avl_t *avl, site_t *site) {
    assert(avl != NULL); // In case the object is not initialized

    if (_insert_node(&avl->root, site) == ERROR) return ERROR;
    

    ++avl->amnt_sites;
    return SUCCESS;
}

/*
    Searches for a `key` in an `avl_t*` and returns 
    if has found it or not.

    @param avl tree to be searched on
    @param key key to be found
*/
bool avl_search(avl_t *avl, int key) {
    assert(avl != NULL); // In case the object is not initialized

    return _search_node(avl->root, key) != NULL ? FOUND : NOT_FOUND;
}

/*
    Prints an `avl_t*` instance

    @param avl tree to be printed
    @param order traversal printing order
    @param format printing format
    @param outfile pointer to outstream
*/
void avl_print(avl_t *avl, printing_order order, printing_format format, FILE *outfile) {
    assert(avl != NULL); // In case the object is not initialized

    if (avl->amnt_sites == 0) { // If there are no sites
        printf("Árvore de sites vazia\n");
        return;
    }

    if (order == PREORDER)
        return _print_preorder(avl->root, format, outfile);
    if (order == INORDER)
        return _print_inorder(avl->root, format, outfile);
    if (order == POSTORDER)
        return _print_postorder(avl->root, format, outfile);
}


/*
    Deletes an `avl_t*` instance.

    @param avl address to the tree
    @param delete_site if TRUE, deletes the `site_t*` in a node; if FALSE, does not
*/
void avl_delete(avl_t **avl, bool delete_site) {
    assert(*avl != NULL); // In case the object is not initialized

    _delete_all_nodes(&(*avl)->root, delete_site);
    free(*avl);
    *avl = NULL;
}

/*
    Deletes an `no_t*` containing a speficied
    `key` from a `avl_t*`


    @param avl address to an `avl_t*` instance
    @param key key of the `site_t` to be deleted 
*/
bool avl_delete_site(avl_t *avl, int key) {
    assert(avl != NULL); // In case the object is not initialized

    if (_delete_node(&avl->root, key) == ERROR) return ERROR;
    

    --avl->amnt_sites;
    return SUCCESS;
}

/*
    Searches and returns the `site_t*`
    (if it exists) given its `key` 

    @param avl tree to be searched
    @param key key of the site to be searched
*/
site_t *avl_get_site(avl_t *avl, int key) {
    assert(avl != NULL); // In case the object is not initialized

    no_t *node;
    if ((node = _search_node(avl->root, key)) == NULL) return NULL;

    return node->site;
}

/*
    Returns the amount of sites in a `avl_t*`

    @param avl tree instance
*/
int avl_get_amnt_sites(avl_t *avl) {
    assert(avl != NULL); // In case the object is not initialized
    
    return avl->amnt_sites;
}

/*
    Searches for an `keyword` in a tree and 
    returns a array of `site_t*` containing 
    it.

    @param avl tree to be searched
    @param keyword keyword to be searched
    @param amnt_matches address to an int storing the amount of matches
*/
site_t** avl_search_keyword(avl_t *avl, string keyword, int *amnt_matches) {
    assert(avl != NULL); // In case the object is not initialized

    *amnt_matches = 0;
    _get_amnt_matches(avl->root, keyword, amnt_matches);    
    site_t **match_sites = malloc(sizeof(site_t*)*(*amnt_matches));
    
    int i = 0;
    _match_sites_with_keyword(avl->root, keyword, match_sites, *amnt_matches, &i);

    return match_sites;
}


/*
    Returns a list of the mosti 
    `site_t*` suggid on a 
    `keyword`

    @param avl tree instance
    @param keyword base keyword
    @param max_amnt_suggestions max amount of suggestions to be returned
*/
site_t** get_most_relevant_suggestions(avl_t *avl, string keyword, int max_amnt_suggestions) {
    assert(avl != NULL && keyword != NULL); // In case the object is not initialized

    return _get_most_relevant_suggestions(avl, keyword, max_amnt_suggestions);
}
