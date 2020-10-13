#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "site.h"

enum _data_columns { // Values indexes
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


static int verify_values(char **values, int amnt_keywords);


/*
    Verifies if the values are within the site especifications
*/
static int verify_values(char **values, int amnt_keywords) {
    int flag = SUCCESS;
    if (strlen(values[KEY]) > MAX_KEY_DIGTS) {
		printf("%s\n\n", values[KEY]);
        printf("Erro ao ler arquivo csv: chave primária acima do limite\n");
        flag = ERROR;
    }

    if (strlen(values[NAME]) > MAX_NAME_LEN) {
        printf("Erro ao ler arquivo csv: nome do site acima do limite\n");
        flag = ERROR;
    }

    int relevancy = atoi(values[RELEVANCY]);
    if (relevancy > MAX_RELEVANCY || relevancy < MIN_RELEVANCY) {
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

/*
    Initializes a site instance.
*/
SITE *site_init(char **values, int amnt_values) {
    int amnt_keywords = amnt_values - 4;
    if (verify_values(values, amnt_keywords) == ERROR) { // If there is any error with the values given
        return NULL;
    }

    SITE *site = malloc(sizeof(SITE));
    if (site == NULL) { 
        return NULL; 
    }

    site->key = atoi(values[KEY]);
    site->name = my_strdup(values[NAME]);
    site->relevancy = atoi(values[RELEVANCY]);
    site->link = my_strdup(values[LINK]);
    site->amnt_keywords = amnt_keywords;

    site->keywords = malloc(sizeof(char *) * MAX_KEYWORDS);
    for (int i = 0; i < amnt_keywords; ++i) {
        site->keywords[i] = my_strdup(values[KEYWORDS + i]);
    }

	site_print(site, stdout);

    return site;
}

/*
    Deletes an site instance.
*/
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
    free((*site)->keywords);
    (*site)->keywords = NULL;
    
    free(*site);
    *site = NULL;
}   

/*
    Returns the key of an site instance
*/
int site_get_key(SITE *site) {
    if (site == NULL) {
        printf("Erro ao acessar chave: objeto vazio\n");
        exit(EXIT_FAILURE);
    }

    return site->key;
}

/*
    Inserts an new keyword into an site instance. Returns -1 in case of error and 0 if the operation is a success.
*/
boolean site_insert_keyword(SITE *site, char *keyword) {
    if (site == NULL) {
        return ERROR;
    }

    if (site->amnt_keywords == MAX_KEYWORDS) {
        return ERROR;
    }

    site->keywords[site->amnt_keywords] = my_strdup(keyword);
    ++site->amnt_keywords;

    return SUCCESS;
}

/*
    Prints the values of an site instance
*/
void site_print(SITE *site, FILE *f_out) {
    if (site == NULL) { 
        return;
    }

    fprintf(f_out, "____________________________________\n");
    fprintf(f_out, "Chave: %d\n", site->key);
    fprintf(f_out, "Nome: %s\n", site->name);
    fprintf(f_out, "Relevância: %d\n", site->relevancy);
    fprintf(f_out, "Link: %s\n", site->link);
    
    fprintf(f_out, "Palavras-chave: ");
    for (int i = 0; i < site->amnt_keywords; ++i) {
        fprintf(f_out, "%s", site->keywords[i]);
        if (i != site->amnt_keywords - 1) {
            fprintf(f_out, ", ");
        }
    }
    fprintf(f_out,"\n____________________________________\n");

}

/*
    Updates the relevância of an site instance
*/
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

/* 
    Prints the information about a site in csv format
*/
void site_print_in_csv_format(SITE *site, FILE *f_out) {
    if (site == NULL) {
        return;
    }

    fprintf(f_out, "%d", site->key);
    fprintf(f_out, ",%s", site->name);
    fprintf(f_out, ",%d", site->relevancy);
    fprintf(f_out, ",%s", site->link);

    for (int i = 0; i < site->amnt_keywords; ++i) {
        fprintf(f_out, ",%s", site->keywords[i]);
    }

    fprintf(f_out, "\n");
}
