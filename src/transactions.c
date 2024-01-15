#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "dictionary.h"
#include "transaction.h"

#define FILE_NAME "data/wallet.tran"

/*
int gl_size; // number of trans.
st_transaction *gl_trans;
*/
int transaction_save(st_transaction trans){

  // @todo : verif trans is all-right.

  FILE * f = fopen(FILE_NAME, "ab");
  fwrite(&trans, sizeof(st_transaction), 1, f);
  fclose(f);
  return 0; 
}

void transaction_read_all(){
  st_transaction trans;
  FILE * f = fopen(FILE_NAME, "rb");
  while(!feof(f)){
    fread(&trans, sizeof(st_transaction),1,f);
    transaction_print(trans);
  }
  return;
}


/**
 * @brief sum all the same token in one transaction.
 * Should allocate memory equal to numbers of tokens.
 * @return st_transaction*, that is a list of transaction that 
 * contain sum of fiat and token amount for each token.
 * Timestamp of transactions contains list size.
 */
st_transaction * transaction_sum(void){
  st_transaction * t_list = malloc(sizeof(st_transaction));
  int t_list_size = 0;
  st_transaction t;
  FILE * f = fopen(FILE_NAME,"rb");
  while(!feof(f)){
    fread(&t, sizeof(st_transaction),1,f);
    // parse t_list 
    int index;
    for(index = 0; index < t_list_size; index++){
      // Do nothing, will init index to the good position.
      if(t_list[index].token_id == t.token_id) break;
    }
    if(index == t_list_size){// creat new entrie
      if(index != 0){
        //if index == 0 it’s the firs malloc we should populate.
        t_list = realloc(t_list, (1+index)*sizeof(st_transaction));
      }
      t_list[index] = t;
      t_list_size++;
    }
    else{
      t_list[index].token += t.token;
      t_list[index].fiat += t.fiat;
    }
  }
  // replacting timestamp by the list size ^^. malin l’abeille.
  for(int i = 0; i<t_list_size; i++){
    t_list[i].timestamp = (time_t) t_list_size;
  }
  return t_list;
}

void transaction_sum_print(st_transaction * t_list){
  if(t_list[0].timestamp >= 255 ){
    printf("ERROR t_list.timestamp is >= 255\n");
    exit(1);
  }
  uint8_t t_list_size = (uint8_t)t_list[0].timestamp;
  for(uint8_t i = 0; i < t_list_size; i++){
    printf("\t%d - %lf - %lf €\n",t_list[i].token_id,t_list[i].token,t_list[i].fiat);
  }
}



void transaction_print(st_transaction t){
    printf("Tr : %ld [%d], %lf, %lf\n",     
    t.timestamp,
    t.token_id,
    t.token,
    t.fiat);
}

