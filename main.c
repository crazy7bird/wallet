#include<stdio.h>
#include"inc/transaction.h"
#include"inc/dictionary.h"


int main(void){
  st_transaction tr = {(time_t) 0x00000001, 0x02, 0x03, 0x04};
  file_separator_init("data.bin");
  printf("S : %ld \n", file_separator_get());
  transaction_init();
  //transaction_add(time(NULL), 1, 100, 10);
  //transaction_add(time(NULL), 1, 100, 8);
  transaction_save(tr,"data.bin");
  transactions_print();
}