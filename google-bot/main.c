#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "site_list.h"
#include "utils.h"
#include "site.h"

int main() {
    SLIST *s_list = slist_init();

    if (s_list == NULL) { // Caso não consiga inicializar a lista
        printf("Erro ao iniciar bot\n");
        return EXIT_FAILURE;
    }

    printf("Bem vindo ao googlebot!!\n");

    while (1) { // Loop principal do programa - mantêm este ativo
        printf("\nO que você deseja fazer?\n");
        printf("1 - Inserir sites\n");
        printf("2 - Remover site\n");
        printf("3 - Atualizar a referência de um site\n");
        printf("4 - Adicionar uma palavra-chave a um site\n");
        printf("5 - Mostrar banco de dados\n");
        printf("6 - Finalizar o programa\n");
        
     
        int option = read_num(stdin);
        if (option == 1) {
            printf("Arquivo de leitura: ");
            char *filename = read_line(stdin);
            insert_site(s_list, filename);
            
            free(filename);
        } 
        if (option == 2) {
            printf("Chave do site: ");
            int key = read_num(stdin);
            remove_site(s_list, key);
        }
        if (option == 3) {
            printf("Chave do site: ");
            int key = read_num(stdin);
            update_relevancy(s_list, key);
        }
        if (option == 4) {
            printf("Chave do site: ");
            int key = read_num(stdin);
            insert_keyword(s_list, key);
        }
        if (option == 5) {
            slist_print(s_list);
        }
        if (option == 6) {
            break;
        }             
        else {
            printf("Comando Inválido\n");
        }
      
    }

    printf("Programa finalizado\n");
    slist_delete(&s_list);

    return EXIT_SUCCESS;
}