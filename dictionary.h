#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
/**
* @file : dictionary.h
* @author : crazybird
* @version : 1
* @brief : defines the dictionary system used in the program.
*/

#include<stint.h>

/*
*
*/
typedef struct st_dictionary st_dictionary;
struct st_dictionary {
 int size;
 char ** token;
}




#endif /* #ifndef _DICTIONARY_H_*
