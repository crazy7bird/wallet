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

int transaction_add(time_t time, uint8_t token_id, double token, double fiat);

int transaction_save(st_transaction trans, char * path);

int transaction_read(int number, int pos, char* path);

void transactions_print();

int transaction_init();


#endif /*_TRANSACTION_H*/