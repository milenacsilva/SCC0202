#include <stdio.h>
#include <stdlib.h>

#include "commands.h"

// void start_bot(void) {

// }

/* Função que lê um arquivo e insere no programa */
void insert_site(SLIST *s_list, char *filename) {
    FILE *f_in = open_file(filename, "r");
    int amnt_values;
    char *line;

    while ((line = read_line(f_in)) != NULL) { // Itera por todas as linhas do arquivo
        char **values = get_values(line, "[,\\]", &amnt_values);
        
        int amnt_keywords = amnt_values - 4; // TODO: Comentario
        if (verify_values(values, amnt_keywords) == SUCCESS) { // Caso tenha algum erro de formatação
            SITE *site = site_init(values, amnt_keywords);
            if (site != NULL && slist_insert_sorted_site(s_list, site) == SUCCESS) {
                printf("Site adicionado com sucesso\n");
            }
        } 
        else {
            printf("Valores passados para o site estão fora do padrão\n");
        }
        free_values(values, amnt_values);
        free(line);
    }

    fclose(f_in);
}

/* Função que remove um site do programa */
void remove_site(SLIST *s_list, int key) {
    if (slist_remove_site(s_list, key) == SUCCESS) {
        printf("Site removido com sucesso\n");
    }
}

/* Função que adiciona uma keyword em um site existente no programa */
void insert_keyword(SLIST *s_list, int key) {
    printf("Qual palavra chave gostaria de adicionar?");
    char *keyword = read_line(stdin);

    SITE *site = slist_get_site(s_list, key);
    if (site != NULL && site_insert_keyword(site, keyword) == SUCCESS) {
        printf("Palavra chave inserida com sucesso\n");
    }

    free(keyword);
}

/* Função que atualiza a relevância ed um site do programa */
void update_relevancy(SLIST *s_list, int key) {
    printf("Qual é a nova relevância? ");
    int relevancy = read_num(stdin);
    
    SITE *site = slist_get_site(s_list, key);
    if (site !=  NULL && site_update_relevancy(site, relevancy) == SUCCESS) {
        printf("Relevância atualizada com sucesso\n");
    }
}