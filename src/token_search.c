#include "../inc/token_search.h"
#include "../inc/gecko_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define FILE_NAME "data/token_list.csv"

/**
 * @brief Convert the Json url : [{}]into \\n
 * @return int number of token available on coingecko.
 */
/* id,symbol,name */
static void line_converter(token_search * t){
    int len = strlen(t->token_list);
    t->n_token_list = 0;
    int y = 0;
    for(int i =0; i<len ; i++){
        if(strncmp(&t->token_list[i], "\"id\":", 5)==0) i+= 5;
        if(strncmp(&t->token_list[i], "\"symbol\":", 9)==0) i+= 9;
        if(strncmp(&t->token_list[i], "\"name\":", 7)==0) i+= 7;
        char c = t->token_list[i];
        if(c == '{' ||
           c == '[' ||
           c == ']' ||
           c == '"') continue;
        if(c == '}'){
            c = '\n';
            i+=2; // remove the extra , 
            t->n_token_list++;
        }
        t->token_list[y]=c;
        y++;
    }
    t->token_list[y] = '\0';
    t->token_list = realloc(t->token_list, strlen(t->token_list)*sizeof(char));
    t->token_list_size = strlen(t->token_list);
    return;
}

/**
 * @brief Parse str till \n then return the next char
 * 
 * @param buffer 
 * @return char* 
 */
static char* next_line(char* buffer){
    // @todo : test if start by \n 
    if(*buffer == '\n')buffer++;
    while(*buffer != '\n' && *buffer != '\0')buffer++;
    if(*buffer == '\0' || buffer[1] == '\0') return NULL;
    return &buffer[1];
    // @todo : investigate why :
    // return buffer++ do not work !
}
/**
 * @brief like next_line but for comma.
 * Will also return on \n
 * 
 * @param buffer 
 * @return char* 
 */
static char* next_comma(char* buffer){
    if(*buffer == '\n')buffer++;
    while(*buffer != '\n' && *buffer != ',' && *buffer != '\0')buffer++;
    if(*buffer == '\0' || buffer[1] == '\0') return NULL;
    return ++buffer;
}


static void search_init(token_search * t){
    if(t->token_search != NULL){ //use previous result for new search
       free(t->token_list);
       t->token_list = t->token_search;
       t->n_token_list = t->n_token_search;
       t->token_list_size = t->token_search_size;
       t->n_token_search = 0;
       t->token_search_size = 0;
    }
    t->token_search = calloc(t->token_list_size,sizeof(char));
}
static void search_clear(token_search * t){
    size_t size = strlen(t->token_search);
    t->token_search[size-1] = '\0'; //clear the last \n
    t->token_search = realloc(t->token_search, size *sizeof(char));
    t->token_search_size = strlen(t->token_search);
}

void token_search_by_id(token_search *t , char* id){
    //We will realloc later.
    //t->token_search = calloc(t->token_list_size,sizeof(char)); //calloc for init all memory to 0
    search_init(t);
    char * index = t->token_list;
    size_t strlen2cmp = strlen(id);
    for(int i = 0; i<t->n_token_list; i++){
        char * line_end = next_line(index);
        if(strncmp(index,id,strlen2cmp) == 0){
            strncat(t->token_search,index,(line_end - index));
            t->n_token_search ++;
        }
        index = line_end;
    }
    search_clear(t);
    return;
}

void token_search_by_symbol(token_search *t, char* symbol){
    search_init(t);
    char * index = t->token_list;
    char * search_index = t->token_list;
    size_t strlen2cmp = strlen(symbol);
    for(int i = 0; i<t->n_token_list; i++){
        search_index = next_comma(index);
        char * line_end = next_line(index);
        if(strncmp(search_index,symbol,strlen2cmp) == 0){
            strncat( t->token_search,index,(line_end - index));
            t->n_token_search ++;
        }
        index = line_end;
    }
    search_clear(t);
    return;
}

void token_search_by_name(token_search *t, char* name){
    search_init(t);
    char * index = t->token_list;
    char * search_index = t->token_list;
    size_t strlen2cmp = strlen(name);
    for(int i = 0; i<t->n_token_list; i++){
        search_index = next_comma(index);
        search_index = next_comma(search_index);
        char * line_end = next_line(index);
        if(strncmp(search_index,name,strlen2cmp) == 0){
            strncat( t->token_search,index,(line_end - index));
            t->n_token_search ++;
        }
        index = line_end;
    }
    search_clear(t);
    return;
}

static int file_exist(){
    return access(FILE_NAME, F_OK) + 1; 
    // access return -1 if dont exist and 0 if exist -1+1 = 0 if dont exist and 0+1 = 1 if exist.
}

void token_list_update(){
    token_search t;
    if(file_exist())remove(FILE_NAME);
    t.token_list = token_list();
    line_converter(&t);
    FILE * f = fopen(FILE_NAME, "w");
    fputs(t.token_list,f);
    fclose(f);
}

static void token_list_load(token_search * t){
    FILE * f = fopen(FILE_NAME, "r");

    fseek( f , 0L , SEEK_END);
    long lSize = ftell( f );
    rewind( f );
    t->token_list = calloc( 1, lSize+1 );
    /* copy the file into the buffer */
    fread( t->token_list , lSize, 1 , f);
    fclose(f);

    //Update vars 
    t->token_list_size = strlen(t->token_list);
    t->n_token_list = 0; // @todo update this var 
    for(int i =0; i<t->token_list_size ; i++)if(t->token_list[i] == '\n')t->n_token_list++;

    return;
}

void token_search_free(token_search * t){
    if(t->token_list != NULL)free(t->token_list);
    if(t->token_search != NULL)free(t->token_search);
    free(t);
    return;
}


static void list_print(char * buffer){
    int line_count = 0;
    printf("%d : ",line_count++);
    while(*buffer != '\0')
    {
        printf("%c",*buffer);
        if(*buffer == '\n' && buffer[1] != '\0'){
            printf("%d : ",line_count++);
        }
        buffer++;
    }
    printf("\n");
    return;
}

void token_list_print(token_search *t){
    list_print(t->token_list);
}

void token_search_print(token_search *t){
    list_print(t->token_search);
}

token_search * token_search_init(){
    token_search * t = calloc(1, sizeof(token_search));
    if(!file_exist()){
        token_list_update();
    }
    token_list_load(t);
    return t;
}