#include <stdio.h>
#include <stdlib.h>

#include "commands.h"

void insert_site(SLIST *s_list) {
    printf("De que arquivo deseja ler os sites? ");
    char *filename = read_line(stdin);
    FILE *f_in = open_file(filename, "r");

    char *line;
    while ((line = read_line(f_in)) != NULL) {
        int amnt_values;
        char **values = get_values(line, "[,\\]", &amnt_values);
        
        if (verify_values(values, amnt_values - 4) != 0) {
            printf("Site fora dos padrões\n");
            free_values(values, amnt_values);
            free(line);
            continue;
        }

        SITE *site = site_init(values, amnt_values - 4);
        if (site != NULL && slist_insert_sorted_site(s_list, site) == SUCCESS) {
            printf("Site adicionado com sucesso\n");
        }
        
        free_values(values, amnt_values);
        free(line);
    }

    free(filename);
    fclose(f_in);
}

void remove_site(SLIST *s_list) {
    printf("Qual a chave do site? ");
    char *tmp_key = read_line(stdin);
    int key = atoi(tmp_key);

    if (slist_remove_site(s_list, key) == SUCCESS) {
        printf("Site removido com sucesso\n");
    }

    free(tmp_key);
}

void insert_keyword(SLIST *s_list) {
    printf("Qual a chave do site? ");
    char *tmp_key = read_line(stdin);
    int key = atoi(tmp_key);

    printf("Qual palavra chave gostaria de adicionar?");
    char *tmp_keyword = read_line(stdin);

    SITE *site = slist_get_site(s_list, key);
    if (site != NULL && site_insert_keyword(site, tmp_keyword) == SUCCESS) {
        printf("Palavra chave inserida com sucesso\n");
    }

    free(tmp_key);
    free(tmp_keyword);
}

void update_relevancy(SLIST *s_list) {
    printf("Qual a chave do site? ");
    char *tmp_key = read_line(stdin);
    int key = atoi(tmp_key);

    printf("Qual é a nova relevância? ");
    char *tmp_relevancy = read_line(stdin);
    int relevancy = atoi(tmp_relevancy);
    
    SITE *site = slist_get_site(s_list, key);
    if (site !=  NULL && site_update_relevancy(site, relevancy) == SUCCESS) {
        printf("Relevância atualizada com sucesso\n");
    }

    free(tmp_key);
    free(tmp_relevancy);
}