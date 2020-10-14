/*-------------------------------------------------------------*
 |  /\_/\ 			  Trabalho feito por                /\_/\  |
 | ( o.o )	    Milena Corrêa da Silva - 11795401      ( o.o ) |
 |  > ^ <    Lourenço de Salles Roselino - 11796805     > ^ <  |
 *-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "mini_google_bot.h"

int main() {
    MINI_GOOGLE_BOT *bot = mini_google_bot_init();
    mini_google_bot_start(bot);
    mini_google_bot_run(bot);
    mini_google_bot_stop(bot);
    mini_google_bot_delete(&bot);
    return EXIT_SUCCESS;
}
