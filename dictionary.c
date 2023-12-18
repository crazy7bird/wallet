#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

st_dictionary gl_dic;

int add_token(char * new_token){
  size_t size = strlen(new_token);
  gl_dic.token = realloc(gl_dic.size + 1);
  gl_dic.token[gl_dic.size] = malloc((strlen(new_token) + 1)*sizeof(char));
  strcpy(gl_dic.token[gl_dic.size], new_token);
  gl_dic.size++;
  return 0;
}

/**
 * @brief get_ID return the ID number of the token.
 * 
 * @param token_id 
 * @return the ID of the token, -1 if not found.
 */
int get_ID(char * token_id){
  for(int i = 0u ; i < gl_dic.size ; i++){
    if(strcmp(token_id, gl_dic[i]) == 0u) return (int)i;
  }
  return -1;
}

/**
 * @brief Get the token object
 * 
 * @param id 
 * @return char* 
 */
char * get_token(int id){
  if(id < 0 ){
    printf("get_token error id can’t be negative\n");
  }
  if(id >= gl_dic.size){
    printf("get_token error id out of range\n");
  }
  return gl_dic.token[id];
}

int dictionary_init(){
  gl_dic.size = 0;
  gl_dic.token = malloc(sizeof(int));
  gl_dic.token[0] = NULL;
}

int dictionary_deinit(){
  
}

int main(void){
  //main for test purpose.
}
