#ifndef _GECKO_API_H_
#define _GECKO_API_H_
#include "dictionary.h"
// Use from dictionary.h
struct st_token;
struct st_dictionary;

/**
 * @brief give a list of token name.
 * 
 */
// @todo : should disappear.
struct token_api {
  char * name;
  double price;
};

int gecko_api(int argc, char** argv);
int price_update(st_dictionary * dictionnary);


#endif /*_GECKO_API_H_*/