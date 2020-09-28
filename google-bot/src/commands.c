#include <stdio.h>
#include <stdlib.h>

#include "commands.h"

void insert_site(FILE *f_in, SLIST *s_list) {
    char *line;
    while ((line = read_line(f_in)) != NULL) {
        int amnt_values;
        char **values = get_data_row(line, "[,\\/]", &amnt_values);
        SITE *site = site_init(values, amnt_values - 4);
        slist_insert_sorted_site(s_list, site);

        for (int i = 0; i < amnt_values; ++i) {
            free(values[i]);
        }
        free(values);
        free(line);
    }
}
