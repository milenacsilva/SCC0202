#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "avl.h"
#include "site.h"
#include "utils.h"
#include "trie.h"

#define height(node) (node == NULL ? 0 : node->height)

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)


typedef struct node *Node;
struct node {
    Site site;
    Node left;
    Node right;
    int height;
};

struct avl {
    Node root;
    int amnt_sites;
};


static void _nullify_node(Node*, bool delete_site);
static Node _init_node(Site);
static Node _search_node(Node root, int key);
static void _rotate_left(Node *root);
static void _rotate_right(Node *root);
static int _get_balance_factor(Node);
static void _balance_tree(Node *root, int key);
static bool _insert_node(Node *root, Site);
static void _print_inorder(Node root, PRINTING_FORMAT, FILE *outfile);
static void _print_preorder(Node root, PRINTING_FORMAT, FILE *outfile);
static void _print_postorder(Node root, PRINTING_FORMAT, FILE *outfile);
static void _delete_all_nodes(Node *root, bool delete_site);
static Node _get_max_value(Node root);
static void _rebalance_tree(Node *root);
static bool _delete_node(Node *root, int key);
static void _get_sites_with_keyword_in_avl(Node root, List, string keyword);
static Trie _get_suggested_keywords(Avl,  string base_keyword);
static void _search_suggestions_in_avl(Node root, Trie suggested_keywords, List suggested_sites);
static List _get_suggestions(Avl, string base_keyword);

/* Frees and nullifies a `Node` instance, but will only delete the `Site` instance in it if specified. */
static void _nullify_node(Node *node, bool delete_site) {
    if (*node == NULL) {
        return;
    }
    
    if (delete_site == TRUE) {
        site_delete(&(*node)->site);
    }

    free(*node);
    *node = NULL;
}

/* Initializes a `Node` instance */
static Node _init_node(Site site) {
    Node new_node = malloc(sizeof(struct node));
    assert(new_node != NULL);

    new_node->left = new_node->right = NULL;
    new_node->site = site;
    new_node->height = 1;
    
    return new_node;
}

/* Searchs for a `Node` instance containing an specific `key` in an `Avl`. */
static Node _search_node(Node root, int key) {
    if (root == NULL) {
        return NULL;
    }
    
    if (site_get_key(root->site) == key) {
        return root;
    }
    
    if (site_get_key(root->site) > key) {
        return _search_node(root->left, key);
    }

    return _search_node(root->right, key);
}

/* Rotates an `Node` instance to te left. */
static void _rotate_left(Node *root) {
    Node new_root = (*root)->right;
    Node subtree = new_root->left;

    (*root)->right = subtree;
    new_root->left = *root;

    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));

    *root = new_root;
}

/* Rotates an `Node` instance to te right. */
static void _rotate_right(Node *root) {
    Node new_root = (*root)->left;
    Node subtree = new_root->right;

    (*root)->left = subtree;
    new_root->right = *root;

    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    new_root->height = 1 + max(height(new_root->left), height(new_root->right));

    *root = new_root;
}

/* Gets the balance factor, meaning the difference in heights from the left and right of a `Node`. */
static int _get_balance_factor(Node node) {
    return node == NULL ? 0 : height(node->left) - height(node->right);
}

/* Balances an `Avl` during the insertion of a new `Node`. */
static void _balance_tree(Node *root, int key) {
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    int balance = _get_balance_factor(*root);

    if (balance > 1 && key < site_get_key((*root)->left->site)){
        return _rotate_right(root); 
    }

    if (balance > 1 && key > site_get_key((*root)->left->site)) {
        _rotate_left(&(*root)->left);
        return _rotate_right(root);
    }

    if (balance < -1 && key > site_get_key((*root)->right->site)) {
        return _rotate_left(root);
    }

    if (balance < -1 && key < site_get_key((*root)->right->site)) {
        _rotate_right(&(*root)->right);
        return _rotate_left(root);
    }
} 

/* Inserts a new `Node` in a `Avl` if its key doesn't already exists. */
static bool _insert_node(Node *root, Site site) {
    if (*root == NULL) {
        *root = _init_node(site);
        return SUCCESS;
    }

    if (site_get_key((*root)->site) == site_get_key(site)) {
        return ERROR;
    }

    int return_flag;
    if (site_get_key((*root)->site) > site_get_key(site)) {
        return_flag = _insert_node(&(*root)->left, site);
    }
    else if (site_get_key((*root)->site) < site_get_key(site)) {
        return_flag = _insert_node(&(*root)->right, site);
    }

    _balance_tree(root, site_get_key(site));
    
    return return_flag;
}

/* Prints an`Avl` in inorder. */
static void _print_inorder(Node root, PRINTING_FORMAT format, FILE *outfile) { 
    if (root == NULL) {
        return;
    }

    _print_inorder(root->left, format, outfile); 

    if (format == TERMINAL) {
        site_print(root->site, outfile);
    } else {
        site_print_in_csv_format(root->site, outfile);  
    }

    _print_inorder(root->right, format,outfile); 
} 

/* Prints an`Avl` in preorder. */
static void _print_preorder(Node root, PRINTING_FORMAT format, FILE *outfile) { 
    if (root == NULL) {
        return; 
    }

    if (format == TERMINAL) {
        site_print(root->site, outfile);
    } else {
        site_print_in_csv_format(root->site, outfile);  
    }

    _print_preorder(root->left, format, outfile); 
    _print_preorder(root->right, format, outfile); 
} 

/* Prints an`Avl` in postorder. */
static void _print_postorder(Node root, PRINTING_FORMAT format, FILE *outfile) { 
    if (root == NULL) {
        return;
    } 

    _print_postorder(root->left, format, outfile); 
    _print_postorder(root->right, format,outfile); 
    
    if (format == TERMINAL) {
        site_print(root->site, outfile);
    } else {
        site_print_in_csv_format(root->site, outfile); 
    }
} 

/* Deletes all `Node`'s recursively from a `Avl`. */
static void _delete_all_nodes(Node *root, bool delete_site) {
    if (*root == NULL) {
        return;
    }
    
    _delete_all_nodes(&(*root)->left, delete_site);
    _delete_all_nodes(&(*root)->right, delete_site);

    _nullify_node(root, delete_site);
}

/* Gets the max key value of a tree starting at its `root`. */
static Node _get_max_value(Node root) {
    if (root == NULL) {
        return NULL;
    }

    return root->right == NULL ? root : _get_max_value(root->right);
}

/* Rebalances an `Avl` after deleting a `Node`. */
static void _rebalance_tree(Node *root) {
    (*root)->height = max(height((*root)->left), height((*root)->right)) + 1; 
    int balance = _get_balance_factor(*root); 

    if (balance > 1 && _get_balance_factor((*root)->left) >= 0) {
        _rotate_right(root); 
    } 

    if (balance > 1 && _get_balance_factor((*root)->left) < 0) { 
        _rotate_left(&(*root)->left); 
        return _rotate_right(root); 
    } 
    
    if (balance < -1 && _get_balance_factor((*root)->right) <= 0) {
        return  _rotate_left(root); 
    }

    if (balance < -1 && _get_balance_factor((*root)->right) > 0) { 
        _rotate_right(&(*root)->right); 
        return  _rotate_left(root); 
    } 
}

/* Searchs and deletes a `Node` given its `key` */
static bool _delete_node(Node *root, int key) {
    if (*root == NULL) return ERROR;
    
    if (site_get_key((*root)->site) > key)  _delete_node(&(*root)->left, key);
    else if (site_get_key((*root)->site) < key) _delete_node(&(*root)->right, key);
    else {
        Node successor;
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
        Site tmp = (*root)->site;
        (*root)->site = successor->site;
        (*root)->left = successor->left;
        
        successor->site = tmp;
        _nullify_node(&successor, TRUE);
    }

    if (*root == NULL) return SUCCESS;

    _rebalance_tree(root);
    return SUCCESS;
}

/* Gets a `List` of `Site`s containing a `keyword` from an `Avl`. */ 
static void _get_sites_with_keyword_in_avl(Node root, List s_list, string keyword) {
    if (root == NULL) return;

    if (site_search_keyword(root->site, keyword) == FOUND) {
        list_insert_site(s_list, root->site);
    }
    _get_sites_with_keyword_in_avl(root->left, s_list, keyword);
    _get_sites_with_keyword_in_avl(root->right, s_list, keyword);
} 

/* Gets a `Trie` instance of all keywords from the sites with `base_keyword`. */
static Trie _get_suggested_keywords(Avl avl,  string base_keyword) {
    List matches_with_base_keyword = avl_search_keyword(avl, base_keyword); // n^2

    Trie suggested_keywords = trie_init(); 
    trie_insert_word(suggested_keywords, base_keyword);

    Site cur_site;
    while ((cur_site = list_get_cur_site(matches_with_base_keyword)) != NULL) {       // n 
        
        int amnt_keywords = site_get_amnt_keywords(cur_site);
        string *keywords = site_get_keywords(cur_site);   
        for (int j = 0; j < amnt_keywords; ++j) {
            trie_insert_word(suggested_keywords, keywords[j]);
        }
        list_go_to_next_site(matches_with_base_keyword);
    }

    list_delete(&matches_with_base_keyword);
    return suggested_keywords;
}

/* Gets a list of `suggested_sites` that contain any word in the `suggested_keywords`. */
static void _search_suggestions_in_avl(Node root, Trie suggested_keywords, List suggested_sites) {
    if (root == NULL) return;

    int amnt_keywords = site_get_amnt_keywords(root->site);
    string *keywords = site_get_keywords(root->site);
    for (int i = 0; i < amnt_keywords; ++i) { 
        if (trie_search_word(suggested_keywords, keywords[i]) == FOUND) {
            list_insert_site(suggested_sites, root->site); 
            break;
        }
    }
    _search_suggestions_in_avl(root->left, suggested_keywords, suggested_sites);
    _search_suggestions_in_avl(root->right, suggested_keywords, suggested_sites);
}

/* Get a suggestion of `Site`s based on a `keyword`. */
static List _get_suggestions(Avl avl, string base_keyword) {
    Trie suggested_keywords = _get_suggested_keywords(avl, base_keyword);
    List suggested_sites = list_init();
    _search_suggestions_in_avl(avl->root, suggested_keywords, suggested_sites);
    trie_delete(&suggested_keywords);

    return suggested_sites;
}

/* Initializes an `Avl` instance. */
Avl avl_init() {
    Avl avl = malloc(sizeof(struct avl));
  
    assert(avl != NULL); // In case cannot initialize the instance

    avl->root = NULL;
    avl->amnt_sites = 0;

    return avl;
}

/* Inserts a new `site` in the `avl`. */
bool avl_insert(Avl avl, Site site) {
    assert(avl != NULL); // In case the object is not initialized

    if (_insert_node(&avl->root, site) == ERROR) {
        return ERROR;
    }

    ++avl->amnt_sites;
    return SUCCESS;
}

/* Searches for a `key` in an `Avl` and returns if has found it or not. */
bool avl_search(Avl avl, int key) {
    assert(avl != NULL); // In case the object is not initialized

    return _search_node(avl->root, key) != NULL ? FOUND : NOT_FOUND;
}

/* Prints an `Avl` instance. */
void avl_print(Avl avl, PRINTING_ORDER order, PRINTING_FORMAT format, FILE *outfile) {
    assert(avl != NULL); // In case the object is not initialized

    if (avl->amnt_sites == 0) { // If there are no sites
        printf("Árvore de sites vazia\n");
        return;
    }
    if (order == PREORDER) {
        return _print_preorder(avl->root, format, outfile);
    }
    if (order == INORDER) {
        return _print_inorder(avl->root, format, outfile);
    }
    if (order == POSTORDER) {
        return _print_postorder(avl->root, format, outfile);
    }
}

/* Deletes an `Avl` instance. */
void avl_delete(Avl *avl, bool delete_site) {
    assert(*avl != NULL); // In case the object is not initialized

    _delete_all_nodes(&(*avl)->root, delete_site);
    free(*avl);
    *avl = NULL;
}

/* Deletes an `Node` containing a specified `key` from a `avl`. */
bool avl_delete_site(Avl avl, int key) {
    assert(avl != NULL); // In case the object is not initialized

    if (_delete_node(&avl->root, key) == ERROR) {
        return ERROR;
    }
    --avl->amnt_sites;
    return SUCCESS;
}

/* Searches and returns the `Site` (if it exists) given its `key`. */
Site avl_get_site(Avl avl, int key) {
    assert(avl != NULL); // In case the object is not initialized
    
    Node node;
    if ((node = _search_node(avl->root, key)) == NULL) {
        return NULL;
    }
    return node->site;
}

/* Returns the amount of sites in an `Avl`. */
int avl_get_amnt_sites(Avl avl) {
    assert(avl != NULL); // In case the object is not initialized
    return avl->amnt_sites;
}

/* Searchs for an `keyword` in a tree and returns a array of `Site` containing it. */ 
List avl_search_keyword(Avl avl, string keyword) {
    assert(avl != NULL);
    List matches = list_init();
    _get_sites_with_keyword_in_avl(avl->root, matches, keyword);
    return matches;
}

/* Get a suggestion of `Site`s based on a `keyword`. */
List get_suggestions(Avl avl, string keyword, int max_amnt_suggestions) {
    assert(avl != NULL && keyword != NULL);
    return _get_suggestions(avl, keyword);
}
