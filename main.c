#include<stdio.h>
#include"inc/transaction.h"
#include"inc/dictionary.h"
#include"inc/gecko_api.h"
#include"inc/token_search.h"

int main(int argc, char** argv){


    test();return 0;

    token_list_init();
    //token_list_print();
    char* search1 = token_search_by_id("bitcoin");
    printf("%s\n",search1);
    free(search1);
    char* search2= token_search_by_symbol("btc");
    printf("%s\n",search2);
    free(search2);

    char* search3= token_search_by_name("Cronos");
    printf("%s\n",search3);
    free(search3);

   token_list_free();
   return 0;

  //st_token t = {"bitcoin", "btc", "Bitcoin"};
  //st_token t = {"ethereum", "eth", "Ethereum"};
  dictionary_load();
  dictionary_print();
  //dictionary_add_token(&t);
  dictionary_price_update();
  dictionary_print();

  return 0;
}