#include<stdio.h>
#include"inc/transaction.h"
#include"inc/dictionary.h"
#include"inc/gecko_api.h"

int main(int argc, char** argv){

  st_token t = {"bitcoin", "btc", "Bitcoin"};
  dictionary_add_token(&t);

  return 0;
}