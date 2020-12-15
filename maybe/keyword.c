#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "keyword.h"
#include "utils.h"


#define height(node) (node == NULL ? 0 : node->height)

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)


typedef struct node *Node;
struct node {
    string keyword;
    Node left;
    Node right;
    int height;
};

struct k_avl {
    Node root;
    int amnt_sites;
};

static void _nullify_node(Node *node);
static Node _init_node(string);
static Node _search_node(Node root, string keyword);
static void _rotate_left(Node *root);
static void _rotate_right(Node *root);
static int _get_balance_factor(Node node);
static void _balance_tree(Node *root, string keyword);
static bool _insert_node(Node *root, string);
static void _delete_all_nodes(Node *root);

/* Frees and nullifies a `Node` instance, but will only delete the `string` instance in it if specified. */
static void _nullify_node(Node *node) {
    if (*node == NULL) {
        return;
    }

    free(*node);
    *node = NULL;
}

/* Initializes a `Node` instance */
static Node _init_node(string keyword) {
    Node new_node = malloc(sizeof(struct node));
    assert(new_node != NULL);

    new_node->left = new_node->right = NULL;
    new_node->keyword = keyword;
    new_node->height = 1;
    
    return new_node;
}

/* Searchs for a `Node` instance containing an specific `keyword` in an `KeywordAvl`. */
static Node _search_node(Node root, string keyword) {
    if (root == NULL) {
        return NULL;
    }
    
    if (strcmp(root->keyword, keyword) == 0) {
        return root;
    }
    
    if (strcmp(root->keyword, keyword) > 0) {
        return _search_node(root->left, keyword);
    }

    return _search_node(root->right, keyword);
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

/* Balances an `KeywordAvl` during the insertion of a new `Node`. */
static void _balance_tree(Node *root, string keyword) {
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    int balance = _get_balance_factor(*root);

    if (balance > 1 && strcmp((*root)->left->keyword, keyword) > 0){
        return _rotate_right(root); 
    }

    if (balance > 1 && strcmp((*root)->left->keyword, keyword) < 0) {
        _rotate_left(&(*root)->left);
        return _rotate_right(root);
    }

    if (balance < -1 && strcmp((*root)->right->keyword, keyword) < 0) {
        return _rotate_left(root);
    }

    if (balance < -1 && strcmp((*root)->right->keyword, keyword) > 0) {
        _rotate_right(&(*root)->right);
        return _rotate_left(root);
    }
} 

/* Inserts a new `Node` in a `KeywordAvl` if its keyword doesn't already exists. */
static bool _insert_node(Node *root, string keyword) {
    if (*root == NULL) {
        *root = _init_node(keyword);
        return SUCCESS;
    }

    if (strcmp((*root)->keyword, keyword) == 0) {
        return ERROR;
    }

    int return_flag;
    if (strcmp((*root)->keyword, keyword) > 0) {
        return_flag = _insert_node(&(*root)->left, keyword);
    }
    else if (strcmp((*root)->keyword, keyword) < 0) {
        return_flag = _insert_node(&(*root)->right, keyword);
    }

    _balance_tree(root, keyword);
    
    return return_flag;
}

/* Deletes all `Node`'s recursively from a `KeywordAvl`. */
static void _delete_all_nodes(Node *root) {
    if (*root == NULL) {
        return;
    }
    
    _delete_all_nodes(&(*root)->left);
    _delete_all_nodes(&(*root)->right);

    _nullify_node(root);
}

/* Initializes an `KeywordAvl` instance. */
KeywordAvl keyword_avl_init() {
    KeywordAvl k_avl = malloc(sizeof(struct k_avl));
  
    assert(k_avl != NULL); // In case cannot initialize the instance

    k_avl->root = NULL;
    k_avl->amnt_sites = 0;

    return k_avl;
}

/* Inserts a new `keyword` in the `k_avl`. */
bool keyword_avl_insert(KeywordAvl k_avl, string keyword) {
    assert(k_avl != NULL); // In case the object is not initialized

    if (_insert_node(&k_avl->root, keyword) == ERROR) {
        return ERROR;
    }

    ++k_avl->amnt_sites;
    return SUCCESS;
}

/* Searches for a `keyword` in an `KeywordAvl` and returns if has found it or not. */
bool keyword_avl_search(KeywordAvl k_avl, string keyword) {
    assert(k_avl != NULL); // In case the object is not initialized

    return _search_node(k_avl->root, keyword) != NULL ? FOUND : NOT_FOUND;
}

/* Deletes an `KeywordAvl` instance. */
void keyword_avl_delete(KeywordAvl *k_avl) {
    assert(*k_avl != NULL); // In case the object is not initialized

    _delete_all_nodes(&(*k_avl)->root);
    free(*k_avl);
    *k_avl = NULL;
}
