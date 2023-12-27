#include<stdio.h>
#include"inc/transaction.h"
#include"inc/dictionary.h"
#include"inc/gecko_api.h"

int main(int argc, char** argv){

  //st_token t = {"bitcoin", "btc", "Bitcoin"};
  st_token t = {"ethereum", "eth", "Ethereum"};
  dictionary_load();
  dictionary_print();
  dictionary_add_token(&t);
  dictionary_print();

  return 0;
}