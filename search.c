#include<stdio.h>
#include"inc/token_search.h"




int main(int argc, char** argv){

  printf("hello from search\n");

  token_search *t;

  token_list_init();
  //printf("%s\n",token_search_by_id("ethereum"));
  token_list_print(token_search_by_id("ethereum"));

  return 0;
}