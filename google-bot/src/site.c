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

// int verify_data_rows(char **data_row, int amnt_keywords) {
//     int errors = 0;

//     if (strlen(data_row[KEY]) > MAX_KEY_DIGTS) {
//         perror("Erro: chave primária acima do limite\n");
//         ++errors;
//     }

//     if (strlen(data_row[NAME]) > MAX_NAME_LEN) {
//         perror("Erro: nome do site acima do limite\n");
//         ++errors;
//     }

//     if (atoi(data_row[RELEVANCY]) > MAX_RELEVANCY || atoi(data_row[RELEVANCY]) < MIN_RELEVANCY) {
//         perror("Erro: relevância fora dos limites\n");       
//         ++errors; 
//     }

//     if (strlen(data_row[LINK]) > MAX_LINK_LEN) {
//         perror("Erro: link do site acima do limite\n");
//         ++errors;
//     }

//     if (amnt_keywords > MAX_KEYWORDS) {
//         perror("Erro: máximo de keywords atingido\n");
//         ++errors;
//     }

//     for (int i = KEYWORDS; i < amnt_keywords + KEYWORDS; ++i) {
//         if (strlen(data_row[i]) > MAX_KEYWORD_LEN) {
//             perror("Erro: palavra-chave maior que o esperado\n");
//             ++errors;
//         }
//     }

//     return errors;
// }

/* Função que inicializa a instância de um site */
SITE *site_init(char **data_row, int amnt_keywords) {
    SITE *site = malloc(sizeof(SITE));
    if (site == NULL) {
        return NULL; 
    }


    site->key = atoi(data_row[KEY]);
    site->name = strdup(data_row[NAME]);
    site->relevancy = atoi(data_row[RELEVANCY]);
    site->link = strdup(data_row[LINK]);
    site->amnt_keywords = amnt_keywords;

    for (int i = 0; i < amnt_keywords; ++i) {
        site->keywords[i] = strdup(data_row[KEYWORDS + i]);
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
        perror("Erro ao acessar chave: objeto vazio\n");
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

    site->keywords[site->amnt_keywords] = keyword;
    ++site->amnt_keywords;

    return SUCCESS;
}

void site_print(SITE *site) {
    printf("chave=%d\n", site->key);
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
