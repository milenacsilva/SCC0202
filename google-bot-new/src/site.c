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
    char *keywords[MAX_KEYWORDS];
    short amnt_keywords;
};

/* Função que verifica os valores de entradas para os sites, ou
seja se estão dentro das especificações */
int verify_values(char **values, int amnt_keywords) {
    int errors = 0;

    if (strlen(values[KEY]) > MAX_KEY_DIGTS) {
        printf("Erro ao ler arquivo csv: chave primária acima do limite\n");
        ++errors;
    }

    if (strlen(values[NAME]) > MAX_NAME_LEN) {
        printf("Erro ao ler arquivo csv: nome do site acima do limite\n");
        ++errors;
    }

    if (atoi(values[RELEVANCY]) > MAX_RELEVANCY || atoi(values[RELEVANCY]) < MIN_RELEVANCY) {
        printf("Erro ao ler arquivo csv: relevância fora dos limites\n");       
        ++errors; 
    }

    if (strlen(values[LINK]) > MAX_LINK_LEN) {
        printf("Erro ao ler arquivo csv: link do site acima do limite\n");
        ++errors;
    }

    if (amnt_keywords > MAX_KEYWORDS) {
        printf("Erro ao ler arquivo csv: máximo de keywords atingido\n");
        ++errors;
    }

    for (int i = KEYWORDS; i < amnt_keywords + KEYWORDS; ++i) {
        if (strlen(values[i]) > MAX_KEYWORD_LEN) {
            printf("Erro ao ler arquivo csv: palavra-chave maior que o esperado\n");
            ++errors;
        }
    }

    return errors;
}

/* Função que inicializa a instância de um site */
SITE *site_init(char **values, int amnt_keywords) {
    SITE *site = malloc(sizeof(SITE));
    if (site == NULL) {
        return NULL; 
    }

    site->key = atoi(values[KEY]);
    site->name = strdup(values[NAME]);
    site->relevancy = atoi(values[RELEVANCY]);
    site->link = strdup(values[LINK]);
    site->amnt_keywords = amnt_keywords;

    for (int i = 0; i < amnt_keywords; ++i) {
        site->keywords[i] = strdup(values[KEYWORDS + i]);
    }

    return site;
}

/* Função para deletar a instância de um site */
void site_delete(SITE **site) {
    if (*site == NULL) {
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
    
    free(*site);
}   

/* Função que retorna a chave de um site */
int site_get_key(SITE *site) {
    if (site == NULL) {
        printf("Erro ao acessar chave: objeto vazio\n");
        exit(EXIT_FAILURE);
    }

    return site->key;
}

/* Função que adiciona uma nova keyword a um site */
boolean site_insert_keyword(SITE *site, char *keyword) {
    if (site == NULL) {
        return ERROR;
    }

    if (site->amnt_keywords == MAX_KEYWORDS) {
        return ERROR;
    }

    site->keywords[site->amnt_keywords] = strdup(keyword);
    ++site->amnt_keywords;

    return SUCCESS;
}

/* Função que printa a instância de um site */
void site_print(SITE *site) {
    if (site == NULL) {
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
    if (site == NULL) {
        return ERROR;
    }

    if (relevancy > MAX_RELEVANCY || relevancy < MIN_RELEVANCY) {
        return ERROR;
    }

    site->relevancy = relevancy;
    return SUCCESS;
}
