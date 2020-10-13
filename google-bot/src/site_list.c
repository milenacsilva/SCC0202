#include <stdio.h>
#include <stdlib.h>

#include "site_list.h"

typedef struct no NO;
struct no {
    SITE *site;
    NO *next;
};

struct sitelist {
    NO *head;
    int amnt_sites;
};


static boolean insert_node(SITELIST *s_list, SITE *site);
static boolean remove_node(SITELIST *s_list, int key);
static NO *pop_node(SITELIST *s_list, int key);


/*
    Initialites an list of sites.
*/
SITELIST *sitelist_init() {
    SITELIST *s_list = malloc(sizeof(SITELIST));

    if (s_list ==  NULL) {
        return NULL;
    }

    s_list->amnt_sites = 0;
    s_list->head = NULL;

    return s_list;
}

/*
    Deletes an instance of a site.
*/
void sitelist_delete(SITELIST **s_list) {
    if (*s_list == NULL) {
        return;
    }

    while ((*s_list)->head != NULL) {
        NO *tmp_head = (*s_list)->head;
        (*s_list)->head = (*s_list)->head->next;

        site_delete(&tmp_head->site);
        free(tmp_head);
        tmp_head = NULL;
    }

    free(*s_list);
    *s_list = NULL;
}

/*
    Inserts an ordered node.
*/
static boolean insert_node(SITELIST *s_list, SITE *site) {
    NO **last_node = NULL;
    NO **current_node = &(s_list->head);

    for (int i = 0; i < s_list->amnt_sites; ++i) { // Procura o index da inserção
        if (site_get_key((*current_node)->site) == site_get_key(site)) { // Caso já exista a chave
            printf("Erro ao adicionar site: chave já existente\n");
            return ERROR;
        }
        if (site_get_key((*current_node)->site) > site_get_key(site)) { // Caso for maior, coloca antes
            break;
        }
        last_node = current_node;
        current_node = &(*current_node)->next;
    }

    NO *new_node = malloc(sizeof(NO)); // Cria um novo nó
    if (new_node == NULL) {
        printf("Erro ao adicionar site: novo nó não inicializado\n");
        return ERROR;
    }

    new_node->site = site;
    new_node->next = *current_node;
    *current_node = new_node;

    return SUCCESS;
}

/*
    Inserts an site in the list. Returns -1 for error and 0 for success.
    Callers cannot free the site instansce alone after inserting it.
*/
boolean sitelist_insert_site(SITELIST *s_list, SITE *site) {
    if (s_list == NULL || site == NULL) {  
        printf("Erro ao inserir novo site: objeto não inicializado\n");
        return ERROR;
    }

    if (s_list->amnt_sites > MAX_AMNT_SITES) {
        printf("Erro ao inserir novo site: lista cheia\n");
        return ERROR;
    }

    if (insert_node(s_list, site) == ERROR) {
        return ERROR;
    }

    ++s_list->amnt_sites;
    return SUCCESS;
}

/*
    Removes a node of an site list.
*/
static boolean remove_node(SITELIST *s_list, int key) {
    NO *last_node = NULL;
    NO *current_node = s_list->head;

    for (int i = 0; i < s_list->amnt_sites; ++i) {
        if (site_get_key(current_node->site) == key) {
            break;
        }
        last_node = current_node;
        current_node = current_node->next;
    }

    if (current_node == NULL) {
        printf("Erro ao remover site: chave não existe\n");
        return ERROR;
    } 

    site_delete(&current_node->site);
    
    NO *next_node = current_node->next;
    if (current_node == s_list->head) { // If its the head element
        s_list->head = next_node;
    }
    else { 
        last_node->next = next_node;
    }

    free(current_node);
    return SUCCESS;
}

/* 
    Pops the node from the list and returns it given a key.
*/
static NO *pop_node(SITELIST *s_list, int key) {
    NO *last_node = NULL;
    NO *current_node = s_list->head;
  
    for (int i = 0; i < s_list->amnt_sites; ++i) {
        if (site_get_key(current_node->site) == key) {
            break;
        }
        last_node = current_node;
        current_node = current_node->next;
    }

    if (current_node == NULL) {
        return NULL;
    } 

    NO *next_node = current_node->next;
    if (current_node == s_list->head) { // If its the head element
        s_list->head = next_node;
    }
    else { 
        last_node->next = next_node;
    }

    return current_node; 
}

/*
    Removes an instance os an site from de list. Returns -1 for error and 0 for success.
*/
boolean sitelist_remove_site(SITELIST *s_list, int key) {
    if (s_list == NULL) { 
        printf("Erro ao remover site: objeto não inicializado\n");
        return ERROR;
    }

    if (remove_node(s_list, key) == ERROR) {
        return ERROR;
    }

    --s_list->amnt_sites;
    return SUCCESS;
}

/* 
    Returns an site instance for the site. 
    Callers cannot free the return value by theirselves.
*/
SITE *sitelist_get_site(SITELIST *s_list, int key) {
    if (s_list == NULL) {
        printf("Erro ao retornar site: lista não inicializada\n");
        return NULL;
    }

    for (NO *curr = s_list->head; curr != NULL; curr = curr->next) {
        if (site_get_key(curr->site) == key) {
            return curr->site;
        }
    }

    printf("Site não encontrado\n");
    return NULL;
}

/*
    Prints the list of sites.
*/
void sitelist_print(SITELIST *s_list) {
    if (s_list == NULL) { 
        printf("Erro ao imprimir a lista: objeto não inicializado\n");
        return;
    } 

    if (s_list->amnt_sites == 0) {
        printf("Lista de sites vazia\n");
        return;
    }

    for (NO *curr = s_list->head; curr != NULL; curr = curr->next) {
		printf("teste\n");
        site_print(curr->site, stdout);
    }
}

/*
    Writes de list of file to an csv formatted file 
*/
void sitelist_write_to_csv_file(SITELIST *s_list, FILE *f_out) {
    if (s_list == NULL) { 
        printf("Erro ao imprimir a lista: objeto não inicializado\n");
        return;
    } 

    if (s_list->amnt_sites == 0) {
        printf("Lista de sites vazia\n");
        return;
    }

    for (NO *curr = s_list->head; curr != NULL; curr = curr->next) {
        site_print_in_csv_format(curr->site, f_out);
    }
}

/* 
    Verifies if a site is full. Returs -1 for erros, 0 for true and 1 for false.
*/
int sitelist_is_full(SITELIST *s_list) {
    if (s_list == NULL) {
        return ERROR;
    }

    if (s_list->amnt_sites == MAX_AMNT_SITES) {
        return TRUE;
    }

    return FALSE;
}

/*
    Verifies if a site list is empty. Returs -1 for erros, 0 for true and 1 for false.
*/
int sitelist_is_empty(SITELIST *s_list) {
    if (s_list == NULL) {
        return ERROR;
    }

    if (s_list->amnt_sites == 0) {
        return TRUE;
    }

    return FALSE;
}

