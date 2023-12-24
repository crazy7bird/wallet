#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "dictionary.h"
#include "transaction.h"

/*
int gl_size; // number of trans.
st_transaction *gl_trans;
*/
int transaction_save(st_transaction trans, char * path){

  // @todo : verif trans is all-right.

  FILE * f = fopen(path, "ab");
  fwrite(&trans, sizeof(st_transaction), 1, f);
  fclose(f);
  return 0; 
}

void transaction_read_all(char* path){
  st_transaction trans;
  long int s_pos = file_separator_get();
  FILE * f = fopen(path, "rb");
  fseek(f, s_pos, SEEK_SET);
  while(!feof(f)){
    printf("fread ret : %ld \n",fread(&trans, sizeof(st_transaction),1,f));
    transaction_print(trans);
  }
  return;
}

void transaction_print(st_transaction t){
    printf("Tr : %ld [%d], %lf, %lf\n",     
    t.timestamp,
    t.token_id,
    t.token,
    t.fiat);
}

