#ifndef _TOKEN_SEARCH_H_
#define _TOKEN_SEARCH_H_
/**
 * @file token_search.h
 * @author your name (you@domain.com)
 * @brief This module provide search functions.
 *        It’s use the query function from gecko_api.h
 * @version 0.1
 * @date 2023-12-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdlib.h>

typedef struct token_search token_search;
struct token_search{
  char* token_list;
  size_t token_list_size;
  int n_token_list;
  char* token_search;
  size_t token_search_size;
  int n_token_search;
};

token_search* token_search_init(void);
void token_list_update();
void token_search_free(token_search* t );
void token_list_print(token_search *t);
void token_search_print(token_search *t);
void token_search_by_id(token_search*, char* id);
void token_search_by_symbol(token_search*, char* symbol);
void token_search_by_name(token_search*, char* name);

#endif /* _TOKEN_SEARCH_H_ */