#include<stdio.h>
#include"inc/token_search.h"
#include <unistd.h>
#include <stdbool.h>

struct option{
  bool update_flag;
  char* id_flag;
  char* symbol_flag;
  char* name_flag;
  bool all_flag;
}option_default = {false,NULL,NULL,NULL,false};
typedef struct option option;

static option  options_manager(int argc, char** argv){
  int opt;
  option o = option_default;
  while( (opt = getopt(argc,argv,"ui:s:n:a")) != -1){
    switch(opt){
        case 'u':
            o.update_flag = true;
            break;
        case 'i':
            o.id_flag = optarg;
            break;
        case 's':
            o.symbol_flag = optarg;
            break;
        case 'n':
            o.name_flag = optarg;
            break;
        case 'a' :
            o.all_flag = true;
            break;
        default :
            exit(1);
    } 
  }
  return o;
}

int main(int argc, char** argv){

  option o = options_manager(argc, argv);

  printf("Options : %d %s %s %s %d\n",o.update_flag,o.id_flag,o.symbol_flag,o.name_flag,o.all_flag);
  //return 0;
  printf("hello from search\n");

  if(o.update_flag) token_list_update();

  token_search *t = token_search_init();

  if(o.id_flag != NULL){
    token_search_by_id(t,o.id_flag);
  }
  else if(o.symbol_flag != NULL){
    token_search_by_symbol(t,o.symbol_flag);
  }
  else if(o.name_flag != NULL){
    token_search_by_name(t,o.name_flag);
  }
  token_search_print(t);
  token_search_free(t);

  return 0;
}
