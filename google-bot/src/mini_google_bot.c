/*-------------------------------------------------------------*
 |  /\_/\ 			  Trabalho feito por                /\_/\  |
 | ( o.o )	    Milena Corrêa da Silva - 11795401      ( o.o ) |
 |  > ^ <    Lourenço de Salles Roselino - 11796805     > ^ <  |
 *-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "mini_google_bot.h"
#include "avl.h"
#include "parser.h"
#include "list.h"

// #include "site_list.h"

struct mini_google_bot {
    Avl avl;
};

static bool insert_sites_from_csv_file(Avl avl, string filename);
static void insert_sites_from_stdin(Avl avl);
static void remove_site(Avl avl, int key);
static void insert_keyword(Avl avl, int key);
static void update_relevancy(Avl avl, int key);

/* 
    Insert all sites upon reading a csv file.
*/
static bool insert_sites_from_csv_file(Avl avl, string filename) {
    FILE *f_in = open_file(filename, "r");
    int amnt_values;
    
    Reader reader = parser_reader_init(CSV_PATTERN);
    string line;

    while ((line = readline(f_in)) != NULL) { // Loops through all lines/value pairs from file
        parser_read(reader,line);

        Site site = site_init(reader->values, reader->amnt_values);
        if (site != NULL && avl_insert(avl, site) == SUCCESS) printf("Site adicionado com sucesso\n");
        else {
            printf("Erro ao adicionar o site\n");
            site_delete(&site);
        }

        parser_clean_reader(reader);
        free(line);
    }

    parser_reader_delete(&reader);

    fclose(f_in);
    return SUCCESS;
}

/*
    Inserts a new site manually, by readig from stdin 
*/
static void insert_sites_from_stdin(Avl avl) {
    string values[MAX_AMNT_VALUES] = {0};
    
    printf("Chave: ");
    values[KEY] = readline(stdin);
    
    printf("Nome: ");
    values[NAME] = readline(stdin);
    
    printf("Relevância (1-1000): ");
    values[RELEVANCY] = readline(stdin);
    
    printf("Link: ");
    values[LINK] = readline(stdin);
    
    int amnt_keywords;
    while (TRUE) {
        printf("Quantidade de palavras-chave: ");
        amnt_keywords = readnum(stdin);
        
        if (amnt_keywords <= MAX_KEYWORDS) break;
        printf("Valor inválido! Digite um número de 1 a 10\n/");
    }
 
    for (int i = 0; i < amnt_keywords; ++i) {
        values[KEYWORDS + i] = readline(stdin);
    }

    Site site = site_init(values, amnt_keywords + 4);
    if (site != NULL && avl_insert(avl, site) == SUCCESS) printf("Site inserido com sucesso");
    else {
        printf("Erro ao adicionar o site\n");
        site_delete(&site);  
    } 

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
static void remove_site(Avl avl, int key) {
    if (avl_delete_site(avl, key) == SUCCESS) printf("Site removido com sucesso\n");
    else printf("Erro ao remover site: site não encontrado\n");
    
}

/*
    Adds an keyword to an existing site in a list
*/
static void insert_keyword(Avl avl, int key) {
    printf("Qual palavra chave gostaria de adicionar?");
    string keyword = readline(stdin);

    Site site = avl_get_site(avl, key);
    if (site != NULL && site_insert_keyword(site, keyword) == SUCCESS) printf("Palavra chave inserida com sucesso\n");
    else printf("Inserir palavra chave\n");
    

    free(keyword);
}

/* 
    Updates the relevancy of an existing site in a list
*/
static void update_relevancy(Avl avl, int key) {
    printf("Qual é a nova relevância? ");
    int relevancy = readnum(stdin);
    
    Site site = avl_get_site(avl, key);
    if (site !=  NULL && site_update_relevancy(site, relevancy) == SUCCESS) printf("Relevância atualizada com sucesso\n");
    else printf("Erro ao atualizar relevância do site\n");
    
}

static void search_for_keyword(Avl avl) {
    printf("Qual palavra chave deseja buscar? ");
    string keyword = readline(stdin);

    SITELIST *matches = avl_search_keyword(avl, keyword);

   
    printf("Os sites relativos a sua busca são\n");
    sitelist_print(matches, 5);

    free(keyword);
    sitelist_delete(&matches);
}

static void get_site_sugestion(Avl avl) {
    printf("Qual é a palvra chave para efetuar a sugestão de sites? ");
    string keyword = readline(stdin);

    SITELIST *suggestions = get_suggestions(avl, keyword, 5);
    sitelist_print(suggestions, 5);


    sitelist_delete(&suggestions);
    free(keyword);
}


/* 
    Initialize an google bot instance
*/
MINI_GOOGLE_BOT *mini_google_bot_init(void) {
    MINI_GOOGLE_BOT *bot = malloc(sizeof(MINI_GOOGLE_BOT));

    if (bot == NULL) {
        return NULL;
    }

    bot->avl = avl_init();

    return bot;
}

/* 
    Starts the google bot by reading from a csv file.
*/
bool mini_google_bot_start(MINI_GOOGLE_BOT *bot) {
    if (bot == NULL) {
        printf("Erro ao incializar google bot: objeto vazio\n");
        return ERROR;
    }

    printf("+-------------------------------------------------+\n");
    printf("|               INCIANDO GOOGLE BOT...            |\n");
    printf("+-------------------------------------------------+\n");
    printf("Arquivo de entrada: ");

    string filename = readline(stdin);
    if (insert_sites_from_csv_file(bot->avl, filename) == ERROR) {
        return ERROR;
    }
    free(filename);

    return SUCCESS;
}

/* 
    Runs the google bot.
*/
bool mini_google_bot_run(MINI_GOOGLE_BOT *bot) {
    while (TRUE) { 
        printf("\nO que você deseja fazer? (Digite o número equivalente ao comando)\n");
        printf("0 - Inserir site manualmente\n");
        printf("1 - Inserir sites a partir de um csv\n");
        printf("2 - Remover site\n");
        printf("3 - Atualizar a relevância de um site\n");
        printf("4 - Adicionar uma palavra-chave a um site\n");
        printf("5 - Mostrar banco de dados\n");
        printf("6 - Buscar palavra chave\n");
        printf("7 - Sugestão de sites\n");
        printf("8 - Finalizar o programa\n");
     
        int option = readnum(stdin, .amnt_terminators=2,
                                    .terminators=(int[]){'\n', EOF});
        if (option == 0) {
            insert_sites_from_stdin(bot->avl);
        }
        else if (option == 1) {
            printf("Arquivo de leitura: ");
            string filename = readline(stdin);
            insert_sites_from_csv_file(bot->avl, filename);
            
            free(filename);
        } 
        else if (option == 2) {
            printf("Chave do site: ");
            int key = readnum(stdin);
            remove_site(bot->avl, key);
        }
        else if (option == 3) {
            printf("Chave do site: ");
            int key = readnum(stdin);
            update_relevancy(bot->avl, key);
        }
        else if (option == 4) {
            printf("Chave do site: ");
            int key = readnum(stdin);
            insert_keyword(bot->avl, key);
        }
        else if (option == 5) {
            avl_print(bot->avl, INORDER, TERMINAL, stdout);
        }
        else if (option == 6) {
            search_for_keyword(bot->avl);
        }
        else if (option == 7) {
            get_site_sugestion(bot->avl);
        }
        else if (option == 8) {
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
    string user_choice = readline(stdin);
    
    if (user_choice[0] == 'y') {
        printf("Arquivo de saída: ");
        string filename = readline(stdin);

        FILE *f_out = open_file(filename, "a+");
        avl_print(bot->avl, INORDER, CSV, f_out);

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

    avl_delete(&(*bot)->avl, TRUE);

    free(*bot);
    *bot = NULL;
}


