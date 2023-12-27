#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
/**
* @file : dictionary.h
* @author : crazybird
* @version : 1
* @brief : defines the dictionary system used in the program.
*/

#include<stdint.h>

#define DICT_MAX_CHAR 50u

typedef struct st_token st_token;
struct st_token{
    // uint8_t ID_Key; This ID is determined by it’s position in the dictionary.
    char * token_id;
    char * token_symbol;
    char * token_name;
    double value;
};


int dictionary_add_token(st_token * new_token);
int dictionary_get_ID(char * token);
int dictionary_load();
void dictionary_print();


#endif /* #ifndef _DICTIONARY_H_*/
