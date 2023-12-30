#include<stdio.h>
#include"inc/transaction.h"
#include"inc/dictionary.h"
#include"inc/gecko_api.h"
#include"inc/token_search.h"

int main(int argc, char** argv){


   test_token_list();
   free_token_list();
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