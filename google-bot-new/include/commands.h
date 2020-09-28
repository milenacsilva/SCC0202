#ifndef COMMANDS_H
#define COMMANDS_H

#include "site_list.h"
#include "utils.h"

void insert_site(SLIST *s_list);
void remove_site(SLIST *s_list);
void insert_keyword(SLIST *s_list);
void update_relevancy(SLIST *s_list);

#endif