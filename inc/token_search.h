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

typedef struct token_search token_search;
struct token_search{
  char* token_list;
  size_t token_list_size;
  int n_token_list;
  char* token_search;
  size_t token_search_size;
  int n_token_search;
};

void token_list_init(void);
void token_list_free();
void token_list_print();
char * token_search_by_id(char* id);
char * token_search_by_symbol(char* symbol);
char * token_search_by_name(char* name);

#endif /* _TOKEN_SEARCH_H_ */