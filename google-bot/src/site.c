/*-------------------------------------------------------------*
 |  /\_/\ 			  Trabalho feito por                /\_/\  |
 | ( o.o )	    Milena Corrêa da Silva - 11795401      ( o.o ) |
 |  > ^ <    Lourenço de Salles Roselino - 11796805     > ^ <  |
 *-------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "site.h"

struct site { 
    short key;
    string name;
    short relevancy;
    string link;
    string *keywords;
    short amnt_keywords;
};

static int _verify_values(string *values, int amnt_keywords);
static bool _keywords_binary_search(string *keywords,string key, int min, int max);
static int _verify_values(string *values, int amnt_keywords);
static bool _keywords_binary_search(string *keywords, string key, int min, int max);
static void _swap(string *a, string *b);
static void _heap_constructor(string *base_arr, int arr_len, int father_index);
static void _heapsort_string(string *base_arr, int arr_len);


/* Verifies if the `values` are within the `Site` specifications. */
static int _verify_values(string *values, int amnt_keywords) {
    int flag = SUCCESS;
    if (strlen(values[KEY]) > MAX_KEY_DIGTS) {
        printf("Erro ao ler arquivo csv: chave primária acima do limite\n");
        flag = ERROR;
    }

    if (strlen(values[NAME]) > MAX_NAME_LEN) {
        printf("Erro ao ler arquivo csv: nome do site acima do limite\n");
        flag = ERROR;
    }

    int relevancy = atoi(values[RELEVANCY]);
    if (relevancy > MAX_RELEVANCY || relevancy < MIN_RELEVANCY) {
        printf("Erro ao ler arquivo csv: relevância fora dos limites\n");       
        flag = ERROR;
    }

    if (strlen(values[LINK]) > MAX_LINK_LEN) {
        printf("Erro ao ler arquivo csv: link do site acima do limite\n");
        flag = ERROR;
    }

    if (amnt_keywords > MAX_KEYWORDS) {
        printf("Erro ao ler arquivo csv: máximo de keywords atingido\n");
        flag = ERROR;
    }

    for (int i = KEYWORDS; i < amnt_keywords + KEYWORDS; ++i) {
        if (strlen(values[i]) > MAX_KEYWORD_LEN) {
            printf("Erro ao ler arquivo csv: palavra-chave maior que o esperado\n");
            flag = ERROR;
        }
    }
    return flag;
}

/* Binary searches for a `key` in a `keywords` array. */
static bool _keywords_binary_search(string *keywords, string key, int min, int max) {
    if (min > max) {
        return NOT_FOUND;
    }

    int mid = min + (max - min)/2;

    if (strcmp(keywords[mid], key) == 0) {
        return mid;
    }
    if (strcmp(keywords[mid], key) < 0) {
        return _keywords_binary_search(keywords, key, mid + 1, max);
    }
    return _keywords_binary_search(keywords, key, min, mid - 1);
}

/* Swaps two `string`s. */
static void _swap(string *a, string *b) {
    string tmp = *a;
    *a = *b;
    *b = tmp; 
}
  
/* ucts a heap tree for `_heapsort_string`. */ 
static void _heap_constructor(string *base_arr, int arr_len, int father_index) {
    int largest_index = father_index;
    int left_child_index = 2*father_index + 1;
    int right_child_index = left_child_index + 1;

    if (left_child_index < arr_len && strcmp(base_arr[left_child_index], base_arr[largest_index]) > 0) {
        largest_index = left_child_index;
    }
    if (right_child_index < arr_len  && strcmp(base_arr[right_child_index], base_arr[largest_index]) > 0) {
        largest_index = right_child_index;
    }

    if (largest_index == father_index) {
        return;
    }

    _swap(&base_arr[largest_index], &base_arr[father_index]);
    _heap_constructor(base_arr, arr_len, largest_index);
}

/* Sorts an array of `string`s using heapsort algorithm. */
static void _heapsort_string(string *base_arr, int arr_len) {
    int mid = arr_len/2;
    
    for (int i = mid - 1 ; i >= 0; --i) {
        _heap_constructor(base_arr, arr_len, i);
    }

    for (int i = arr_len - 1; i > 0; --i) {
        _swap(&base_arr[0], &base_arr[i]);
        _heap_constructor(base_arr, i, 0);
    }     
}

/* Initializes a `Site` instance from a `values` arr. */
Site site_init(string *values, int amnt_values) {
    int amnt_keywords = amnt_values - 4;
    if (_verify_values(values, amnt_keywords) == ERROR) { // If there is any error with the values given
        return NULL;
    }

    Site site = malloc(sizeof(struct site));
    assert(site != NULL);

    site->key = atoi(values[KEY]);
    site->name = my_strdup(values[NAME]);
    site->relevancy = atoi(values[RELEVANCY]);
    site->link = my_strdup(values[LINK]);
    site->amnt_keywords = amnt_keywords;

    site->keywords = malloc(sizeof(string) * MAX_KEYWORDS);
    for (int i = 0; i < amnt_keywords; ++i) {
        site->keywords[i] = my_strdup(values[KEYWORDS + i]);
    }
    _heapsort_string(site->keywords, site->amnt_keywords);

    return site;
}

/* Deletes a `Site` instance given its address. */
void site_delete(Site *site) {
    if (*site == NULL) return;

    free((*site)->name);
    (*site)->name = NULL;
    
    free((*site)->link);
    (*site)->link = NULL;

    for (int i = 0; i < (*site)->amnt_keywords; ++i) {
        free((*site)->keywords[i]);
        (*site)->keywords[i] = NULL;
    }
    free((*site)->keywords);
    (*site)->keywords = NULL;
    
    free(*site);
    *site = NULL;
}   

/* Returns the key of an `Site` instance. */
int site_get_key(Site site) {
    assert(site != NULL);

    return site->key;
}

/* Inserts a new `keyword` into a `Site` instance. */
bool site_insert_keyword(Site site, string keyword) {
    assert(site != NULL);

    if (site->amnt_keywords == MAX_KEYWORDS) {
        return ERROR;
    }

    if (_keywords_binary_search(site->keywords, keyword, 0, site->amnt_keywords - 1) == NOT_FOUND) { // If keyword already exists
        return ERROR;
    }

    site->keywords[site->amnt_keywords] = my_strdup(keyword);
    ++site->amnt_keywords;

    _heapsort_string(site->keywords, site->amnt_keywords); // Sorts the keyword after each insertion

    return SUCCESS;
}

/* Prints a `Site` instance to an `outfile`. */
void site_print(Site site, FILE *outfile) {
    if (site == NULL) {
        return;
    }

    fprintf(outfile, "____________________________________\n");
    fprintf(outfile, "Chave: %d\n", site->key);
    fprintf(outfile, "Nome: %s\n", site->name);
    fprintf(outfile, "Relevância: %d\n", site->relevancy);
    fprintf(outfile, "Link: %s\n", site->link);
    
    fprintf(outfile, "Palavras-chave: ");
    for (int i = 0; i < site->amnt_keywords; ++i) {
        fprintf(outfile, "%s", site->keywords[i]);
        if (i != site->amnt_keywords - 1) {
            fprintf(outfile, ", ");
        }
    }
    fprintf(outfile,"\n____________________________________\n");
}

/* Updates the `relevancy` of a `Site` instance. */
bool site_update_relevancy(Site site, int relevancy) {
    assert(site != NULL);

    if (relevancy > MAX_RELEVANCY || relevancy < MIN_RELEVANCY) { 
        return ERROR;
    }

    site->relevancy = relevancy;
    return SUCCESS;
}

/* Prints a `Site` instance in csv format. */
void site_print_in_csv_format(Site site, FILE *outfile) {
    assert(site != NULL);

    fprintf(outfile, "%d", site->key);
    fprintf(outfile, ",%s", site->name);
    fprintf(outfile, ",%d", site->relevancy);
    fprintf(outfile, ",%s", site->link);

    for (int i = 0; i < site->amnt_keywords; ++i) {
        fprintf(outfile, ",%s", site->keywords[i]);
    }

    fprintf(outfile, "\n");
}

/* Searches for a `keyword` in a `Site`'s keywords. */
bool site_search_keyword(Site site, string keyword) {
    assert(site != NULL);
    assert(keyword != NULL);

    int index = _keywords_binary_search(site->keywords, keyword, 0, site->amnt_keywords - 1);
    return index == NOT_FOUND ? NOT_FOUND : FOUND;
}

/* Returns the keywords array from a `Site`. */
string* site_get_keywords(Site site) {
    assert(site != NULL);

    return site->keywords;
}

/* Returns the amount of keywords a a `Site`. */
int site_get_amnt_keywords(Site site) {
    assert(site != NULL);

    return site->amnt_keywords;
}

string site_get_name(Site site) {
    assert(site != NULL);

    return site->name;
}

string site_get_link(Site site) {
    assert(site != NULL);

    return site->link;
}

/* Returns the relevancy of a `Site`. */
int site_get_relevancy(Site site) {
    assert(site != NULL);

    return site->relevancy;
}