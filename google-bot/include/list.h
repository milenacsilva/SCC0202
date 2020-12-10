#ifndef _LIST_H
#define _LIST_H

#include "site.h"

typedef struct sitelist SITELIST;

SITELIST *sitelist_init();
bool sitelist_insert_site(SITELIST *s_list, Site site);
void sitelist_delete(SITELIST **s_list);
void sitelist_print(SITELIST *s_list, int amnt_sites_to_print);

int sitelis_get_amnt_sites(SITELIST *s_list);
void sitelist_go_to_next_site(SITELIST *s_list);
Site sitelist_get_cur_site(SITELIST *s_list);

#endif