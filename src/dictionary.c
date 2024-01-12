#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gecko_api.h"

// Temporary defineds :
#define FILE_NAME "data/wallet.dic"


st_dictionary gl_dic;

static int dictionary_file_append(st_token * new_token){
  // @todo : get name from a file system manager.
  FILE * f = fopen(FILE_NAME, "ab");
  
  fwrite(new_token->token_id, strlen(new_token->token_id) * sizeof(char), 1, f);
  fwrite("\n", sizeof(char), 1, f);

  fwrite(new_token->token_symbol, strlen(new_token->token_symbol) * sizeof(char), 1, f);
  fwrite("\n", sizeof(char), 1, f);

  fwrite(new_token->token_name, strlen(new_token->token_name) * sizeof(char), 1, f);
  fwrite("\n", sizeof(char), 1, f);

  fclose(f);
  return 0;
}

static int dictionary_load_token(st_token * new_token){
  // Check if tocken isn’t in the dictionary.

  // @todo : adding coingecko api check.
  gl_dic.entry = realloc(gl_dic.entry,sizeof(st_token)*(gl_dic.size + 1));
  gl_dic.entry[gl_dic.size].token_id = malloc((strlen(new_token->token_id) + 1)*sizeof(char));
  strcpy(gl_dic.entry[gl_dic.size].token_id, new_token->token_id);
  gl_dic.entry[gl_dic.size].token_symbol = malloc((strlen(new_token->token_symbol) + 1)*sizeof(char));
  strcpy(gl_dic.entry[gl_dic.size].token_symbol, new_token->token_symbol);
  gl_dic.entry[gl_dic.size].token_name = malloc((strlen(new_token->token_name) + 1)*sizeof(char));
  strcpy(gl_dic.entry[gl_dic.size].token_name, new_token->token_name);
  gl_dic.size++;

  return 0;
}

int dictionary_add_token(st_token * new_token){
  // Check if tocken isn’t in the dictionary.
  if(dictionary_get_ID(new_token->token_id) != -1){
    printf("dictionary_add_token : error token already in dictionary\n");
    return -1;
  }
  dictionary_load_token(new_token);
  dictionary_file_append(new_token);
  return 0;
}

int dictionary_get_ID(char * token){
  for(int i = 0u ; i < gl_dic.size ; i++){
    if(strcmp(token, gl_dic.entry[i].token_id) == 0u ||
       strcmp(token, gl_dic.entry[i].token_symbol) == 0u ||
       strcmp(token, gl_dic.entry[i].token_name) == 0u ) return (int)i;
  }
  return -1;
}

st_token * dictionary_get_token(uint8_t ID){
  if(ID < gl_dic.size){
    return &gl_dic.entry[ID];
  }
  return NULL;
  // Branchless programing for fun
  // return gl_dic.entry[ID] *( ID < gl_dic.size );
}

static void clearLine(char *LINE){
  for(int i =0; i<DICT_MAX_CHAR; i++)LINE[i] = '\0';
}

int dictionary_load(){
  FILE * f = fopen(FILE_NAME,"rb");
  char t_id[DICT_MAX_CHAR];
  char t_symbol[DICT_MAX_CHAR];
  char t_name[DICT_MAX_CHAR];
  st_token l_token = {t_id, t_symbol, t_name, 0};
  do{
    // Get token ID
    clearLine(t_id);
    fgets(t_id, DICT_MAX_CHAR, f);
    if(strlen(t_id)<1) break;
    t_id[strlen(t_id) - 1u] = '\0'; //replace \n by \0

    //Get token symbol
    clearLine(t_symbol);
    fgets(t_symbol, DICT_MAX_CHAR, f);
    t_symbol[strlen(t_symbol) - 1u] = '\0'; //replace \n by \0

    //Get token name
    clearLine(t_name);
    fgets(t_name, DICT_MAX_CHAR, f);
    t_name[strlen(t_name) - 1u] = '\0'; //replace \n by \0

    dictionary_load_token(&l_token);
  }while(!feof(f));
  return 0;
}

void dictionary_print(){
  for(int i = 0; i < gl_dic.size ; i++ ){
    printf("ID : %d; Token : %s; %s; %s = %lf\n",i,
    gl_dic.entry[i].token_id,
    gl_dic.entry[i].token_symbol,
    gl_dic.entry[i].token_name,
    gl_dic.entry[i].value);
  }
}

void dictionary_price_update(){
  price_update(&gl_dic);
}