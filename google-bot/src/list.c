#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct node *Node;
struct node {
    Site site;
    Node next;
};

struct list {
    Node head;
    Node cur;
    int amnt_sites;
};

static bool insert_node(List s_list, Site site);

/* Inserts an ordered node. */
static bool insert_node(List s_list, Site site) {
    Node *current_node = &(s_list->head);
    for (int i = 0; i < s_list->amnt_sites; ++i) { 
        if (site_get_key((*current_node)->site) == site_get_key(site)) {  // If the key already exists
            return ERROR;
        }
        if (site_get_relevancy((*current_node)->site) < site_get_relevancy(site)) { // If its bigger, inserts before
            break;
        }
        current_node = &(*current_node)->next;
    }

    Node new_node = malloc(sizeof(*new_node));
    assert(new_node != NULL);
    new_node->site = site;
    new_node->next = *current_node;
    *current_node = new_node;
    
    return SUCCESS;
}

/* Initialites a `List` of sites. */
List list_init() {
    List s_list = malloc(sizeof(*s_list));

    assert(s_list != NULL);
    s_list->amnt_sites = 0;
    s_list->head = NULL;
    s_list->cur = NULL;
    return s_list;
}

/* Inserts an `Site` in the `List`. */
bool list_insert_site(List s_list, Site site) {
    assert(s_list != NULL);
    assert(site != NULL);
      
    if (insert_node(s_list, site) == ERROR) {
        return ERROR;
    }
    s_list->cur = s_list->head;
    ++s_list->amnt_sites;

    return SUCCESS;
}

/* Deletes an instance of `List`. */
void list_delete(List *s_list) {
    if (*s_list == NULL) {
        return;
    }

    while ((*s_list)->head != NULL) {
        Node tmp_head = (*s_list)->head;
        (*s_list)->head = (*s_list)->head->next;

        free(tmp_head);
        tmp_head = NULL;
    }

    free(*s_list);
    *s_list = NULL;
}

/* Prints the `List` of `Site`s. */
void list_print(List s_list, int amnt_sites_to_print) {
    assert(s_list != NULL);

    if (s_list->amnt_sites == 0) {
        printf("Nada foi encontrado :(\n");
        return;
    }

    printf("\nOs sites relativos a sua busca são\n");
    if (amnt_sites_to_print == 0)  {
        amnt_sites_to_print = s_list->amnt_sites;
    }

    int i = 0;
    for (Node curr = s_list->head; curr != NULL && i < amnt_sites_to_print; curr = curr->next, i++) {
        printf("%dº - %s\n", i + 1, site_get_name(curr->site));
        printf("  | %s\n\n", site_get_link(curr->site));
    }
}

/* Go to the next `Site` in the list starting at he `head`. Walks with the `cur` pointer. */
void list_go_to_next_site(List s_list) {
    assert(s_list != NULL);
    if (s_list->cur != NULL) {
        s_list->cur = s_list->cur->next;
    } 
}

/* Returns the `Site` in the `cur` pointer in the list. */
Site list_get_cur_site(List s_list) {
    assert(s_list != NULL);
    if (s_list->cur == NULL) return NULL;

    return s_list->cur->site;
}