#include<stdio.h>
#include"inc/token_search.h"


int main(int argc, char** argv){

  printf("hello from search\n");

  token_search *t = token_search_init();

  token_search_by_id(t,"ethereum");
  token_search_print(t);
  token_search_free(t);

  return 0;
}