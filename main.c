#include<stdio.h>
#include"inc/transaction.h"
#include"inc/dictionary.h"
#include"inc/gecko_api.h"

int main(int argc, char** argv){
  //st_transaction tr = {(time_t) 0xFFFFFFFF, 0x02, 0x03, 0x04};
  file_separator_init("data.bin");
  dictionary_init();

  dictionary_load("data.bin");
  dictionary_print();
  //transaction_save(tr,"data.bin");
  transaction_read_all("data.bin");

  gecko_api(argc, argv);

  return 0;
}