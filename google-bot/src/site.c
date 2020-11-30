/*-------------------------------------------------------------*
 |  /\_/\ 			  Trabalho feito por                /\_/\  |
 | ( o.o )	    Milena Corrêa da Silva - 11795401      ( o.o ) |
 |  > ^ <    Lourenço de Salles Roselino - 11796805     > ^ <  |
 *-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "site.h"

struct _site_t { 
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

static int comparator_function(const void *a, const void *b) {
    return strcmp((string)a, (string)b); 
}

static bool binary_search(string *keywords, string key, int min, int max) {
    if (min > max) return NOT_FOUND;

    int mid = min + (max - min)/2;

    if (strcmp(keywords[mid], key) == 0) return FOUND;

    if (strcmp(keywords[mid], key) < 0)
        return binary_search(keywords, key, mid + 1, max);
    
    return binary_search(keywords, key, min, mid - 1);
}


/*
    Initializes a site instance.
*/
site_t *site_init(char **values, int amnt_values) {
    int amnt_keywords = amnt_values - 4;
    if (verify_values(values, amnt_keywords) == ERROR) { // If there is any error with the values given
        return NULL;
    }

    site_t *site = malloc(sizeof(site_t));
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
    qsort(site->keywords, site->amnt_keywords, sizeof(string), comparator_function);

    return site;
}

/*
    Deletes an site instance.
*/
void site_delete(site_t **site) {
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
int site_get_key(site_t *site) {
    if (site == NULL) {
        printf("Erro ao acessar chave: objeto vazio\n");
        exit(EXIT_FAILURE);
    }

    return site->key;
}

/*
    Inserts an new keyword into an site instance. Returns -1 in case of error and 0 if the operation is a success.
*/
boolean site_insert_keyword(site_t *site, char *keyword) {
    if (site == NULL) {
        return ERROR;
    }

    if (site->amnt_keywords == MAX_KEYWORDS) {
        return ERROR;
    }

    site->keywords[site->amnt_keywords] = my_strdup(keyword);
    ++site->amnt_keywords;

    qsort(site->keywords, site->amnt_keywords, sizeof(string), comparator_function);
    return SUCCESS;
}

/*
    Prints the values of an site instance
*/
void site_print(site_t *site, FILE *outfile) {
    if (site == NULL) { 
        return;
    }

    fprintf(outfile, "____________________________________\n");
    fprintf(outfile, "Chave: %d\n", site->key);
    fprintf(outfile, "Nome: %s\n", site->name);
    fprintf(outfile, "Relevância: %d\n", site->relevancy);
    fprintf(outfile, "Link: %s\n", site->link);
    
    fprintf(outfile, "Palavras-chave: ");
    for (int i = 0; i < site->amnt_keywords; ++i) {
        fprintf(outfile, "%s", site->keywords[i]);
        if (i != site->amnt_keywords - 1) {
            fprintf(outfile, ", ");
        }
    }
    fprintf(outfile,"\n____________________________________\n");

}

/*
    Updates the relevância of an site instance
*/
boolean site_update_relevancy(site_t *site, int relevancy) {
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
void site_print_in_csv_format(site_t *site, FILE *outfile) {
    if (site == NULL) {
        return;
    }

    fprintf(outfile, "%d", site->key);
    fprintf(outfile, ",%s", site->name);
    fprintf(outfile, ",%d", site->relevancy);
    fprintf(outfile, ",%s", site->link);

    for (int i = 0; i < site->amnt_keywords; ++i) {
        fprintf(outfile, ",%s", site->keywords[i]);
    }

    fprintf(outfile, "\n");
}

bool site_search_keyword(site_t *site, string keyword) {
    if (site == NULL || keyword == NULL) {
        printf("Erro ao procurar palavra chave no site: objeto não inicializado\n");
        return ERROR;
    }

    return binary_search(site->keywords, keyword, 0, site->amnt_keywords - 1);
}


string* site_get_keywords(site_t *site){
    if (site == NULL) {
        printf("Erro ao pegar palavras chaves do site: objeto não inicializado\n");
        return NULL;
    }

    return site->keywords;
}

int site_get_amnt_keywords(site_t *site) {
    if (site == NULL) {
        printf("Erro ao pegar quantidades de palavras do site: objeto não inicializado\n");
        return ERROR;
    }
    return site->amnt_keywords;
}

int site_get_relevancy(site_t *site) {
    if (site == NULL) return ERROR;

    return site->relevancy;
}