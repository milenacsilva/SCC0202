/*-------------------------------------------------------------*
 |  /\_/\ 			  Trabalho feito por                /\_/\  |
 | ( o.o )	    Milena Corrêa da Silva - 11795401      ( o.o ) |
 |  > ^ <    Lourenço de Salles Roselino - 11796805     > ^ <  |
 *-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "mini_google_bot.h"
#include "site_list.h"

struct mini_google_bot {
    SITELIST *s_list;
};

static boolean insert_sites_from_csv_file(SITELIST *s_list, char *filename);
static void insert_sites_from_stdin(SITELIST *s_list);
static void remove_site(SITELIST *s_list, int key);
static void insert_keyword(SITELIST *s_list, int key);
static void update_relevancy(SITELIST *s_list, int key);

/* 
    Initialize an google bot instance
*/
MINI_GOOGLE_BOT *mini_google_bot_init(void) {
    MINI_GOOGLE_BOT *bot = malloc(sizeof(MINI_GOOGLE_BOT));

    if (bot == NULL) {
        return NULL;
    }

    bot->s_list = sitelist_init();
    return bot;
}

/* 
    Starts the google bot by reading from a csv file.
*/
boolean mini_google_bot_start(MINI_GOOGLE_BOT *bot) {
    if (bot == NULL) {
        printf("Erro ao incializar google bot: objeto vazio\n");
        return ERROR;
    }

    printf("+-------------------------------------------------+\n");
    printf("|               INCIANDO GOOGLE BOT...            |\n");
    printf("+-------------------------------------------------+\n");
    printf("Arquivo de entrada: ");

    char *filename = read_line(stdin);
    if (insert_sites_from_csv_file(bot->s_list, filename) == ERROR) {
        return ERROR;
    }
    free(filename);

    return SUCCESS;
}

/* 
    Runs the google bot.
*/
boolean mini_google_bot_run(MINI_GOOGLE_BOT *bot) {
    while (TRUE) { 
        printf("\nO que você deseja fazer? (Digite o número equivalente ao comando)\n");
        printf("0 - Inserir site manualmente\n");
        printf("1 - Inserir sites a partir de um csv\n");
        printf("2 - Remover site\n");
        printf("3 - Atualizar a relevância de um site\n");
        printf("4 - Adicionar uma palavra-chave a um site\n");
        printf("5 - Mostrar banco de dados\n");
        printf("6 - Finalizar o programa\n");
     
        int option = read_line_num(stdin);
        if (option == 0) {
            insert_sites_from_stdin(bot->s_list);
        }
        else if (option == 1) {
            printf("Arquivo de leitura: ");
            char *filename = read_line(stdin);
            insert_sites_from_csv_file(bot->s_list, filename);
            
            free(filename);
        } 
        else if (option == 2) {
            printf("Chave do site: ");
            int key = read_line_num(stdin);
            remove_site(bot->s_list, key);
        }
        else if (option == 3) {
            printf("Chave do site: ");
            int key = read_line_num(stdin);
            update_relevancy(bot->s_list, key);
        }
        else if (option == 4) {
            printf("Chave do site: ");
            int key = read_line_num(stdin);
            insert_keyword(bot->s_list, key);
        }
        else if (option == 5) {
            sitelist_print(bot->s_list);
        }
        else if (option == 6) {
            return EXIT_SUCCESS;
        }             
        else {
            printf("Comando Inválido\n");
        } 
    }
}

/*
    Stops the mini google bot
*/
void mini_google_bot_stop(MINI_GOOGLE_BOT *bot) {
    printf("+-------------------------------------------------+\n");
    printf("|            FINALIZANDO GOOGLE BOT...            |\n");
    printf("+-------------------------------------------------+\n");
    printf("Deseja salvar o atual lista de sites[y/n]: ");
    char *user_choice = read_line(stdin);
    
    if (user_choice[0] == 'y') {
        printf("Arquivo de saída: ");
        char *filename = read_line(stdin);

        FILE *f_out = open_file(filename, "a+");
        sitelist_write_to_csv_file(bot->s_list, f_out);

        free(filename);
        fclose(f_out);
    }
    free(user_choice);
}

/*
    Function that deletes an bot instance
*/
void mini_google_bot_delete(MINI_GOOGLE_BOT **bot) {
    if (*bot == NULL) {
        return;
    }

    sitelist_delete(&(*bot)->s_list);

    free(*bot);
    *bot = NULL;
}

/* 
    Insert all sites upon reading a csv file.
*/
static boolean insert_sites_from_csv_file(SITELIST *s_list, char *filename) {
    FILE *f_in = open_file(filename, "r");
    int amnt_values;
    char *line;

    while ((line = read_line(f_in)) != NULL) { // Loops through all lines/value pairs from file
        char **values = parser_reader(line, CSV_PATTERN, &amnt_values);
        
        SITE *site = site_init(values, amnt_values);
        if (site != NULL && sitelist_insert_site(s_list, site) == SUCCESS) {
            printf("Site adicionado com sucesso\n");
        }

        parser_free_values(values, amnt_values);
        free(line);
    }

    fclose(f_in);
    return SUCCESS;
}

/*
    Inserts a new site manually, by readig from stdin 
*/
static void insert_sites_from_stdin(SITELIST *s_list) {
    char *values[MAX_AMNT_VALUES] = {0};
    
    printf("Chave: ");
    values[KEY] = read_line(stdin);
    
    printf("Nome: ");
    values[NAME] = read_line(stdin);
    
    printf("Relevância (1-1000): ");
    values[RELEVANCY] = read_line(stdin);
    
    printf("Link: ");
    values[LINK] = read_line(stdin);
    
    int amnt_keywords;
    while (TRUE) {
        printf("Quantidade de palavras-chave: ");
        amnt_keywords = read_line_num(stdin);
        
        if (amnt_keywords <= MAX_KEYWORDS) break;
        printf("Valor inválido! Digite um número de 1 a 10\n/");
    }
 
    for (int i = 0; i < amnt_keywords; ++i) {
        values[KEYWORDS + i] = read_line(stdin);
    }

    SITE *site = site_init(values, amnt_keywords + 4);
    sitelist_insert_site(s_list, site);

    for (int i = 0; i < MAX_AMNT_VALUES; ++i) {
        if (values[i] != NULL) {
            free(values[i]);
            values[i] = NULL;
        }
    }
}

/*
    Removes an instance of a site
*/
static void remove_site(SITELIST *s_list, int key) {
    if (sitelist_remove_site(s_list, key) == SUCCESS) {
        printf("Site removido com sucesso\n");
    }
}

/*
    Adds an keyword to an existing site in a list
*/
static void insert_keyword(SITELIST *s_list, int key) {
    printf("Qual palavra chave gostaria de adicionar?");
    char *keyword = read_line(stdin);

    SITE *site = sitelist_get_site(s_list, key);
    if (site != NULL && site_insert_keyword(site, keyword) == SUCCESS) {
        printf("Palavra chave inserida com sucesso\n");
    }

    free(keyword);
}

/* 
    Updates the relevancy of an existing site in a list
*/
static void update_relevancy(SITELIST *s_list, int key) {
    printf("Qual é a nova relevância? ");
    int relevancy = read_line_num(stdin);
    
    SITE *site = sitelist_get_site(s_list, key);
    if (site !=  NULL && site_update_relevancy(site, relevancy) == SUCCESS) {
        printf("Relevância atualizada com sucesso\n");
    }
}
