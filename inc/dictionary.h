#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
/**
* @file : dictionary.h
* @author : crazybird
* @version : 1
* @brief : defines the dictionary system used in the program.
*/

/* file separator system */
void file_separator_init(char * path);
long int file_separator_get(void);

int dictionary_add_token(char * new_token);
int dictionary_get_ID(char * token_id);
char * dictionary_get_token(int id);
void dictionary_print();
int dictionary_init();
int dictionary_deinit();
int dictionary_save(char * path);
int dictionary_load(char * path);

#endif /* #ifndef _DICTIONARY_H_*/