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
    int amnt_sites;
};

/* Inicializa uma instância de uma lista de sites */
SLIST *slist_init() {
    SLIST *s_list = malloc(sizeof(SLIST));

    if (s_list ==  NULL) { // Caso dê erro na malloc
        return NULL;
    }

    s_list->amnt_sites = 0;
    s_list->head = NULL;

    return s_list;
}

/* Função que deleta a instância de uma lista de sites */
void slist_delete(SLIST **s_list) {
    if (*s_list == NULL) { // Caso não tenha sido inicializada 
        return;
    }

    while ((*s_list)->head != NULL) { // Deleta cada nó da lista
        NO *tmp_head = (*s_list)->head;
        (*s_list)->head = (*s_list)->head->next;

        site_delete(&tmp_head->site);
        free(tmp_head);
        tmp_head = NULL;
    }

    free(*s_list);
    *s_list = NULL;
}

/* Função privada para colocar um novo nó na lista, de maneira ordenada */
boolean insert_node(SLIST *s_list, SITE *site) {
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

/* Função para inserir um novo site na lista de sites */
boolean slist_insert_sorted_site(SLIST *s_list, SITE *site) {
    if (s_list == NULL || site == NULL) { // Caso alguns dos parâmetros passados não tenha sido inicializado 
        printf("Erro ao inserir novo site: objeto não inicializado\n");
        return ERROR;
    }

    if (s_list->amnt_sites > MAX_AMNT_SITES) { // Se a lista já estiver cheia
        printf("Erro ao inserir novo site: lista cheia\n");
        return ERROR;
    }

    if (insert_node(s_list, site) == ERROR) { // Se der algum erro ao inserir o site
        return ERROR;
    }

    ++s_list->amnt_sites;
    return SUCCESS;
}

/* Função privada para remover um nó da lista */
boolean remove_node(SLIST *s_list, int key) {
    NO *last_node = NULL;
    NO *current_node = s_list->head;
  
    for (int i = 0; i < s_list->amnt_sites; ++i) { // Procura pela chave para remover
        if (site_get_key(current_node->site) == key) {
            break;
        }
        last_node = current_node;
        current_node = current_node->next;
    }

    if (current_node == NULL) { // Caso não ache a chave
        printf("Erro ao remover site: chave não existe\n");
        return ERROR;
    } 

    site_delete(&current_node->site);
    
    NO *next_node = current_node->next;
    if (current_node == s_list->head) { // Caso for o primeiro elemento a ser deletado
        s_list->head = next_node;
    }
    else { // Caso seja os demias elementos 
        last_node->next = next_node;
    }

    free(current_node);
    return SUCCESS;
}

/* Função para remover um site da lista de sites */
boolean slist_remove_site(SLIST *s_list, int key) {
    if (s_list == NULL) { 
        printf("Erro ao remover site: objeto não inicializado\n");
        return ERROR;
    }

    if (remove_node(s_list, key) == ERROR) { // Se não conseguir remover algum site
        return ERROR;
    }

    --s_list->amnt_sites;
    return SUCCESS;
}

SITE *slist_get_site(SLIST *s_list, int key) {
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

/* Função para printar a lista de sites */
void slist_print(SLIST *s_list) {
    if (s_list == NULL) { 
        printf("Erro ao imprimir a lista: objeto não inicializado\n");
        return;
    } 

    if (s_list->amnt_sites == 0) { // Caso não tenha nenhum site 
        printf("Lista de sites vazia\n");
        return;
    }

    for (NO *curr = s_list->head; curr != NULL; curr = curr->next) { // Imprime os sites
        site_print(curr->site);
    }
}