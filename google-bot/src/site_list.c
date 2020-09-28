#include <stdio.h>
#include <stdlib.h>

#include "site_list.h"

typedef struct no NO;
struct no {
    SITE *site;
    NO *next;
};

struct slist {
    NO *head;
    int *primary_keys;
    int amnt_sites;
};

/* Inicializa uma instância de uma lista de sites */
SLIST *slist_init() {
    SLIST *s_list = malloc(sizeof(SLIST));

    if (s_list ==  NULL) {
        return NULL;
    }

    s_list->amnt_sites = 0;
    s_list->primary_keys = malloc(sizeof(int) * MAX_AMNT_SITES);

    return s_list;
}

/* Função que deleta a instância de uma lista de sites */
void slist_delete(SLIST **s_list) {
    if (*s_list == NULL) {
        return;
    }

    free((*s_list)->primary_keys);
    (*s_list)->primary_keys = NULL;

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

void update_primary_keys(SLIST *s_list) {
    NO *aux = s_list->head;
    for (int i = 0; i < s_list->amnt_sites; ++i) {
        s_list->primary_keys[i] = site_get_key(aux->site);
        aux = aux->next;
    }
}

/* Função privada para achar o index de inserção */
int get_insertion_index(int *primary_keys, int amnt_sites, int new_key) {
    int index = binary_search(primary_keys, new_key, 0, amnt_sites - 1);
    
    if (primary_keys[index] != new_key) {
        return index;
    }

    return INVALID_OPERATION;
}

/* Função privada para colocar um novo nó na lista, de maneira ordenada */
boolean insert_sorted_node(NO **head, int index, SITE *site) {
    NO **aux = head;
    for (int i = 0; i < index; ++i) {
        aux = &(*aux)->next;
    }
    
    NO *new_node = malloc(sizeof(NO));
    if (new_node == NULL) {
        return ERROR;
    }

    new_node->site = site;
    new_node->next = *aux;
    *aux = new_node;

    return SUCCESS;
}

/* Função para inserir um novo site na lista de sites */
boolean slist_insert_sorted_site(SLIST *s_list, SITE *site) {
    if (s_list == NULL || site == NULL) {
        return ERROR;
    }

    if (s_list->amnt_sites > MAX_AMNT_SITES) {
        perror("Erro ao inserir novo site: lista cheia\n");
        return ERROR;
    }


    int new_key = site_get_key(site);
    int index = get_insertion_index(s_list->primary_keys, s_list->amnt_sites, new_key);

    if (index == INVALID_OPERATION) {
        perror("Erro ao inserir novo site: site já existente\n");
        return ERROR;
    }

    if (insert_sorted_node(&s_list->head, index, site) != SUCCESS) {
        perror("Erro ao inserir novo nó a lista\n");
        return ERROR;
    }

    s_list->primary_keys[s_list->amnt_sites] = new_key;
    ++s_list->amnt_sites;
    update_primary_keys(s_list);

    return SUCCESS;
}

/* Função privada para achar o index de remoção dado uma chave */
int get_index(int *primary_keys, int amnt_sites, int key) {
    int index = binary_search(primary_keys, key, 0, amnt_sites - 1);

    if (primary_keys[index] == key) {
        return index;
    }

    return INVALID_OPERATION;
}

/* Função privada para remover um nó da lista */
boolean remove_node(SLIST *s_list, int index) {
    NO *last_node = NULL;
    NO *current_node = s_list->head;
  
    for (int i = 0; i < index; ++i) {
        last_node = current_node;
        current_node = current_node->next;
    }

    site_delete(&current_node->site);
    
    NO *next_node = current_node->next;
    if (current_node == s_list->head) {
        printf("ta no inicio\n");
        
        s_list->head = next_node;
        
        free(current_node);
        current_node == NULL;
    }

    else {
        free(current_node);
        last_node->next = next_node;
    }

    return SUCCESS;
}

/* Função para remover um site da lista de sites */
boolean slist_remove_site(SLIST *s_list, int key) {
    if (s_list == NULL) {
        perror("Erro ao remover site: objeto não inicializado\n");
        return ERROR;
    }

    int index = get_index(s_list->primary_keys, s_list->amnt_sites, key);
    
    if (index == INVALID_OPERATION) {
        perror("Erro ao remover site: chave não pertence à nenhum objeto da lista\n");
        return ERROR;
    }

    if (remove_node(s_list, index) == ERROR) {
        perror("Erro ao remover site");
        return ERROR;
    }

    --s_list->amnt_sites;
    update_primary_keys(s_list);

    return SUCCESS;
}

SITE *slist_get_site(SLIST *s_list, int key) {
    if (s_list == NULL) {
        perror("Erro ao retornar site: objeto não inicializado\n");
        return NULL;
    }

    int index = get_index(s_list->primary_keys, s_list->amnt_sites, key);
    if (index == INVALID_OPERATION) {
        perror("Erro ao retornar o site: chave não existe\n");
        return NULL;
    }

    NO *aux = s_list->head;
    for (int i = 0; i < index; ++i) {
        aux = aux->next;
    }

    return aux->site;
}

/* Função para printar a lista de sites */
void slist_print(SLIST *s_list) {
    if (s_list == NULL) {
        perror("Erro ao imprimir a lista de sites: objeto não inicializado\n");
    } 

    if (s_list->amnt_sites == 0) {
        printf("Lista de sites vazia\n");
    }

    for (NO *aux = s_list->head; aux != NULL; aux = aux->next) {
        site_print(aux->site);
    }
}

