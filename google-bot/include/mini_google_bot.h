#ifndef MINI_GOOGLE_BOT_H
#define MINI_GOOGLE_BOT_H

#include "utils.h"

typedef struct mini_google_bot MiniGoogleBot; 

/* 
    Initialize an google bot instance.
*/
MiniGoogleBot *mini_google_bot_init(void);

/* 
    Starts the google bot by reading from a csv file.
*/
bool mini_google_bot_start(MiniGoogleBot *bot);

/* 
    Runs the google bot.
*/
bool mini_google_bot_run(MiniGoogleBot *bot);

/*
    Stops the mini google bot.
*/
void mini_google_bot_stop(MiniGoogleBot *bot);

/*
    Function that deletes an bot instance.
*/
void mini_google_bot_delete(MiniGoogleBot **bot);

#endif