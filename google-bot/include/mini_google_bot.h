#ifndef MINI_GOOGLE_BOT_H
#define MINI_GOOGLE_BOT_H

#include "utils.h"

typedef struct mini_google_bot MINI_GOOGLE_BOT; 

/* 
    Initialize an google bot instance
*/
MINI_GOOGLE_BOT *mini_google_bot_init(void);

/* 
    Starts the google bot by reading from a csv file.

    @param bot mini google bot instance
*/
bool mini_google_bot_start(MINI_GOOGLE_BOT *bot);

/* 
    Runs the google bot.

    @param bot instance of a mini google bot
*/
bool mini_google_bot_run(MINI_GOOGLE_BOT *bot);

/*
    Stops the mini google bot

    @param bot instance of a mini google bot
*/
void mini_google_bot_stop(MINI_GOOGLE_BOT *bot);

/*
    Function that deletes an bot instance

    @param bot address to an bot instance
*/
void mini_google_bot_delete(MINI_GOOGLE_BOT **bot);

#endif