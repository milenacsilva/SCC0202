#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

typedef struct no NO;
struct no {
    Site site;
    NO *next;
};

struct sitelist {
    NO *head;
    NO *cur;
    int amnt_sites;
};


static bool insert_node(SITELIST *s_list, Site site);

/* Inserts an ordered node. */
static bool insert_node(SITELIST *s_list, Site site) {
    NO **current_node = &(s_list->head);
    for (int i = 0; i < s_list->amnt_sites; ++i) { 
        if (site_get_key((*current_node)->site) == site_get_key(site)) { 
            return ERROR;
        }
        if (site_get_relevancy((*current_node)->site) < site_get_relevancy(site)) { 
            break;
        }
        current_node = &(*current_node)->next;
    }

    NO *new_node = malloc(sizeof(NO));
    assert(new_node != NULL);
    new_node->site = site;
    new_node->next = *current_node;
    *current_node = new_node;
    return SUCCESS;
}

/* Initialites an list of sites. */
SITELIST *sitelist_init() {
    SITELIST *s_list = malloc(sizeof(SITELIST));

    if (s_list ==  NULL) {
        return NULL;
    }

    s_list->amnt_sites = 0;
    s_list->head = NULL;

    return s_list;
}

/* Inserts an site in the list. Callers cannot free the site instansce alone after inserting it. */
bool sitelist_insert_site(SITELIST *s_list, Site site) {
    assert(s_list != NULL);
    assert(site != NULL);
      
    if (insert_node(s_list, site) == ERROR) {
        return ERROR;
    }

    s_list->cur = s_list->head;
    ++s_list->amnt_sites;

    return SUCCESS;
}

/* Deletes an instance of a site. */
void sitelist_delete(SITELIST **s_list) {
    if (*s_list == NULL) {
        return;
    }

    while ((*s_list)->head != NULL) {
        NO *tmp_head = (*s_list)->head;
        (*s_list)->head = (*s_list)->head->next;

        free(tmp_head);
        tmp_head = NULL;
    }

    free(*s_list);
    *s_list = NULL;
}

/* Prints the list of sites. */
void sitelist_print(SITELIST *s_list, int amnt_sites_to_print) {
    assert(s_list != NULL);

    if (s_list->amnt_sites == 0) {
        printf("Lista de sites vazia\n");
        return;
    }

    int i = 0;
    for (NO *curr = s_list->head; curr != NULL && i < amnt_sites_to_print; curr = curr->next) {
        site_print(curr->site, stdout);
    }
}

void sitelist_go_to_next_site(SITELIST *s_list) {
    assert(s_list != NULL);
    if (s_list->cur != NULL) {
        s_list->cur = s_list->cur->next;
    } 
}

Site sitelist_get_cur_site(SITELIST *s_list) {
    assert(s_list != NULL);
    if (s_list->cur == NULL) return NULL;
    return s_list->cur->site;
}