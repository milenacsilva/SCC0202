#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "site_list.h"
#include "site.h"

int main() {
    SLIST *s_list = slist_init();

    if (s_list == NULL) {
        printf("Erro ao iniciar bot\n");
        return EXIT_FAILURE;
    }

    printf("Bem vindo ao googlebot!!\n");
    char option;
    do {
        printf("\nO que você deseja fazer?\n"
               "1 - Inserir sites\n"
               "2 - Remover site\n"
               "3 - Atualizar a referência de um site\n"
               "4 - Adicionar uma palavra-chave à um site\n"
               "5 - Mostrar banco de dados\n"
               "6- Finalizar o programa\n");
        
        option = fgetc(stdin);
        fgetc(stdin);

       if (option == '1') 
            insert_site(s_list);
        else if (option == '2')
            remove_site(s_list);
        else if (option == '3')
            update_relevancy(s_list);
        else if (option == '4')
            insert_keyword(s_list);
        else if (option == '5')
            slist_print(s_list);
        else if (option == '6')
            break;
        
    } while(1);

    printf("Programa finalizado\n");
    slist_delete(&s_list);
}