#include<stdio.h>
#include"inc/token_search.h"
#include <unistd.h>
#include <stdbool.h>

#define MAX_SEARCH_OPT 3
struct option{
  bool update_flag;
  char* id_flag;
  char* symbol_flag;
  char* name_flag;
  char order[MAX_SEARCH_OPT];
  bool all_flag;
}option_default = {false,NULL,NULL,NULL,{0},false};
typedef struct option option;

static void order_add(char c, option *o){
    for(int i = 0; i < MAX_SEARCH_OPT; i++){
      if(o->order[i] == 0){
        o->order[i] = c;
        return;
      }
    }
    printf("[ERROR] order buffer is full\n");
    return;
}

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
            order_add('i',&o);
            break;
        case 's':
            o.symbol_flag = optarg;
            order_add('s',&o);
            break;
        case 'n':
            o.name_flag = optarg;
            order_add('n',&o);
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

  printf("Options : %d %s %s %s %.3s %d\n",o.update_flag,o.id_flag,o.symbol_flag,o.name_flag,o.order,o.all_flag);
  //return 0;
  printf("hello from search\n");

  if(o.update_flag) token_list_update();

  token_search *t = token_search_init();
/* wait before do that 
  for(int i= 0; i<MAX_SEARCH_OPT; i++){
    switch(o.order[i]){
      case 'i' :
        token_search_by_id(t,o.id_flag);
        break;
      case 's' :
        token_search_by_id(t,o.id_flag);
        break;
      case 'n' :
        token_search_by_id(t,o.id_flag);
        break;
      default :
        printf("ERROR in order\n");
    }
  }
*/

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
