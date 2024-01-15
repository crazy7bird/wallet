#ifndef _TRANSACTION_H
#define _TRANSACTION_H
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


int transaction_save(st_transaction trans);
void transaction_read_all();
void transaction_print(st_transaction t);

st_transaction * transaction_sum(void);
void transaction_sum_print(st_transaction * t_list);


#endif /*_TRANSACTION_H*/