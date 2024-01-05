#include<stdio.h>
#include"inc/transaction.h"
#include"inc/dictionary.h"
#include"inc/gecko_api.h"
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

struct option{

  char* id_flag; // char* or check for number.

  bool add_flag;
  time_t add_timestamp;
  double add_token;
  double add_fiat;

}option_default = {NULL, false, 0,0,0};
typedef struct option option;

int main(int argc, char** argv){

  //st_token t = {"bitcoin", "btc", "Bitcoin"};
  //st_token t = {"ethereum", "eth", "Ethereum"};
  dictionary_load();
  dictionary_print();
  //dictionary_add_token(&t);
  dictionary_price_update();
  dictionary_print();

  return 0;
}