#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
/**
* @file : dictionary.h
* @author : crazybird
* @version : 1
* @brief : defines the dictionary system used in the program.
*/

int dictionary_add_token(char * new_token);
int dictionary_get_ID(char * token_id);
char * dictionary_get_token(int id);
void dictionary_print();
int dictionary_init();
int dictionary_deinit();

#endif /* #ifndef _DICTIONARY_H_*/
