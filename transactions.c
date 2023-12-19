#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


typedef struct st_transaction st_transaction;
struct st_transaction{
    time_t timestamp;
    uint8_t token_id;
    double token;
    double fiat;
};
int gl_size; // number of trans.
st_transaction *gl_trans;

int transaction_add(time_t time, uint8_t token_id, double token, double fiat){
    
    // @todo secure verif 

    gl_size ++;
    gl_trans = realloc(gl_trans, gl_size * sizeof(st_transaction));
    gl_trans[gl_size -1].timestamp = time;
    gl_trans[gl_size -1].token_id = token_id;
    gl_trans[gl_size -1].token = token;
    gl_trans[gl_size -1].fiat = fiat;

  return 0;
}

void transactions_print(){
    for(int i = 0; i < gl_size; i++){
        printf("Tr : %ld [%d], %lf, %lf\n",     
          gl_trans[i].timestamp,
          gl_trans[i].token_id,
          gl_trans[i].token,
          gl_trans[i].fiat);
    }
}

int transaction_init(){
    gl_size = 0;
    gl_trans = malloc(sizeof(st_transaction));
    gl_trans[0].timestamp = 0;
    gl_trans[0].token_id = 0;
    gl_trans[0].token = 0;
    gl_trans[0].fiat = 0;
    return 0;
}

//main for test purpose.
int main(void){
  transaction_init();
  transaction_add(time(NULL), 1, 100, 10);
  transaction_add(time(NULL), 1, 100, 8);
  transactions_print();
}