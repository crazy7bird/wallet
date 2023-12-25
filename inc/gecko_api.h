#ifndef _GECKO_API_H_
#define _GECKO_API_H_


/**
 * @brief give a list of token name.
 * 
 */
struct token_api {
  char * name;
  double price;
};

int gecko_api(int argc, char** argv);


#endif /*_GECKO_API_H_*/