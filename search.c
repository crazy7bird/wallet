#include<stdio.h>
#include"inc/token_search.h"




int main(int argc, char** argv){

  printf("hello from search\n");

  token_list_init();
  printf("%s\n",token_search_by_id("ethereum"));

  return 0;
}