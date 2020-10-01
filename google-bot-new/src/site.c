#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "site.h"

enum _data_columns {
    KEY,
    NAME,
    RELEVANCY,
    LINK,
    KEYWORDS
};

struct _site {
    short key;
    char *name;
    short relevancy;
    char *link;
    char **keywords;
    short amnt_keywords;
};

/* Função que verifica os valores de entradas para os sites, ou
seja se estão dentro das especificações */
int verify_values(char **values, int amnt_keywords) {
    int flag = SUCCESS;
    if (strlen(values[KEY]) > MAX_KEY_DIGTS) {
        printf("Erro ao ler arquivo csv: chave primária acima do limite\n");
        flag = ERROR;
    }

    if (strlen(values[NAME]) > MAX_NAME_LEN) {
        printf("Erro ao ler arquivo csv: nome do site acima do limite\n");
        flag = ERROR;
    }

    if (atoi(values[RELEVANCY]) > MAX_RELEVANCY || atoi(values[RELEVANCY]) < MIN_RELEVANCY) {
        printf("Erro ao ler arquivo csv: relevância fora dos limites\n");       
        flag = ERROR;

    }

    if (strlen(values[LINK]) > MAX_LINK_LEN) {
        printf("Erro ao ler arquivo csv: link do site acima do limite\n");
        flag = ERROR;
    }

    if (amnt_keywords > MAX_KEYWORDS) {
        printf("Erro ao ler arquivo csv: máximo de keywords atingido\n");
        flag = ERROR;
    }

    for (int i = KEYWORDS; i < amnt_keywords + KEYWORDS; ++i) {
        if (strlen(values[i]) > MAX_KEYWORD_LEN) {
            printf("Erro ao ler arquivo csv: palavra-chave maior que o esperado\n");
            flag = ERROR;
        }
    }
    return flag;
}

/* Função que inicializa a instância de um site */
SITE *site_init(char **values, int amnt_keywords) {
    SITE *site = malloc(sizeof(SITE));
    
    if (site == NULL) { // Caso a site não seja inicializado
        return NULL; 
    }

    site->key = atoi(values[KEY]);
    site->name = strdup(values[NAME]);
    site->relevancy = atoi(values[RELEVANCY]);
    site->link = strdup(values[LINK]);
    site->amnt_keywords = amnt_keywords;

    site->keywords = malloc(sizeof(char*)*MAX_KEYWORDS);
    for (int i = 0; i < amnt_keywords; ++i) {
        site->keywords[i] = strdup(values[KEYWORDS + i]);
    }

    return site;
}

/* Função para deletar a instância de um site */
void site_delete(SITE **site) {
    if (*site == NULL) { // Caso o objeto não tenha sido incializado
        return;
    }

    free((*site)->name);
    (*site)->name = NULL;
    
    free((*site)->link);
    (*site)->link = NULL;

    for (int i = 0; i < (*site)->amnt_keywords; ++i) {
        free((*site)->keywords[i]);
        (*site)->keywords[i] = NULL;
    }
    free((*site)->keywords);
    (*site)->keywords = NULL;
    
    free(*site);
    *site = NULL;
}   

/* Função que retorna a chave de um site */
int site_get_key(SITE *site) {
    if (site == NULL) { // Caso o usuário tenta acessar um site não inicializado
        printf("Erro ao acessar chave: objeto vazio\n");
        exit(EXIT_FAILURE);
    }

    return site->key;
}

/* Função que adiciona uma nova keyword a um site */
boolean site_insert_keyword(SITE *site, char *keyword) {
    if (site == NULL) { // Caso o objeto não tenha sido incializado
        return ERROR;
    }

    if (site->amnt_keywords == MAX_KEYWORDS) { // Caso já tenha sido atingido o máximo de palavras
        return ERROR;
    }

    site->keywords[site->amnt_keywords] = strdup(keyword);
    ++site->amnt_keywords;

    return SUCCESS;
}

/* Função que printa a instância de um site */
void site_print(SITE *site) {
    if (site == NULL) { // Caso o objeto não tenha sido incializado
        return;
    }

    printf("-----------------------\n");
    printf("Chave: %d\n" 
           "Nome: %s\n"
           "Relevância: %d\n"
           "Link: %s\n"
           "Palavras-chave: ", site->key, site->name, site->relevancy, site->link);

    for (int i = 0; i < site->amnt_keywords; ++i) {
        printf("%s, ", site->keywords[i]);
    }
    putchar('\n');
    printf("-----------------------\n");
}

/* Função que atualiza a relevância de um site */
boolean site_update_relevancy(SITE *site, int relevancy) {
    if (site == NULL) { // Caso o objeto nã tenha sido inicializado
        return ERROR;
    }

    if (relevancy > MAX_RELEVANCY || relevancy < MIN_RELEVANCY) { // Caso a referencia esteja fora dos paramêtros
        return ERROR;
    }

    site->relevancy = relevancy;
    return SUCCESS;
}
