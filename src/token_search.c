#include "../inc/token_search.h"
#include "../inc/gecko_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define FILE_NAME "data/token_list.csv"

/**
 * @brief token_list is global var that contain list of all token from coingecko.
 * 
 */
char * gl_token_list = NULL;
int gl_n_token = 0;
size_t gl_token_list_size = 0;
char * gl_token_search = NULL; // Save the search.
/**
 * @brief Convert the Json url : [{}]into \\n
 * @return int number of token available on coingecko.
 */
/* id,symbol,name */
static int line_converter(){
    int len = strlen(gl_token_list);
    int number_token = 0;
    int y = 0;
    for(int i =0; i<len ; i++){
        if(strncmp(&gl_token_list[i], "\"id\":", 5)==0) i+= 5;
        if(strncmp(&gl_token_list[i], "\"symbol\":", 9)==0) i+= 9;
        if(strncmp(&gl_token_list[i], "\"name\":", 7)==0) i+= 7;
        char c = gl_token_list[i];
        if(c == '{' ||
           c == '[' ||
           c == ']' ||
           c == '"') continue;
        if(c == '}'){
            c = '\n';
            i+=2; // remove the extra , 
            number_token++;
        }
        gl_token_list[y]=c;
        y++;
    }
    gl_token_list[y] = '\0';
    gl_token_list = realloc(gl_token_list, strlen(gl_token_list)*sizeof(char));
    gl_token_list_size = strlen(gl_token_list);
    return number_token;
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

static char * search_clear(char * buffer){
    size_t size = strlen(buffer);
    buffer[size-1] = '\0'; //clear the last \n
    buffer = realloc(buffer, size *sizeof(char));
}

char * token_search_by_id(char* id){
    //We will realloc later.
    char * buffer = calloc(gl_token_list_size,sizeof(char)); //calloc for init all memory to 0
    char * index = gl_token_list;
    size_t strlen2cmp = strlen(id);
    for(int i = 0; i<gl_n_token; i++){
        char * line_end = next_line(index);
        if(strncmp(index,id,strlen2cmp) == 0){
            strncat(buffer,index,(line_end - index));
        }
        index = line_end;
    }
    return search_clear(buffer);
}

char * token_search_by_symbol(char* symbol){
    //We will realloc later.
    char * buffer = calloc(gl_token_list_size,sizeof(char)); //calloc for init all memory to 0
    char * index = gl_token_list;
    char * search_index = gl_token_list;
    size_t strlen2cmp = strlen(symbol);
    for(int i = 0; i<gl_n_token; i++){
        search_index = next_comma(index);
        char * line_end = next_line(index);
        if(strncmp(search_index,symbol,strlen2cmp) == 0){
            strncat(buffer,index,(line_end - index));
        }
        index = line_end;
    }
    return search_clear(buffer);
}

char * token_search_by_name(char* name){
    //We will realloc later.
    char * buffer = calloc(gl_token_list_size,sizeof(char)); //calloc for init all memory to 0
    char * index = gl_token_list;
    char * search_index = gl_token_list;
    size_t strlen2cmp = strlen(name);
    for(int i = 0; i<gl_n_token; i++){
        search_index = next_comma(index);
        search_index = next_comma(search_index);
        char * line_end = next_line(index);
        if(strncmp(search_index,name,strlen2cmp) == 0){
            strncat(buffer,index,(line_end - index));
        }
        index = line_end;
    }
    return search_clear(buffer);
}

static int file_exist(){
    return access(FILE_NAME, F_OK) + 1; // access return -1 if dont exist and 0 if exist -1+1 = 0 if dont exist and 0+1 = 1 if exist.
}

static void token_list_update(token_search * t){
    // @ todo : finish un_global prog
    if(file_exist())remove(FILE_NAME);
    t->token_list = token_list();
    t->n_token_list = line_converter();
    FILE * f = fopen(FILE_NAME, "w");
    fputs(t->token_list,f);
    fclose(f);
}

static void token_list_load(){
    FILE * f = fopen(FILE_NAME, "r");

    fseek( f , 0L , SEEK_END);
    long lSize = ftell( f );
    rewind( f );
    gl_token_list = calloc( 1, lSize+1 );
    /* copy the file into the buffer */
    fread( gl_token_list , lSize, 1 , f);
    fclose(f);

    //Update vars 
    gl_token_list_size = strlen(gl_token_list);
    gl_n_token = 0; // @todo update this var 
    for(int i =0; i<gl_token_list_size ; i++)if(gl_token_list[i] == '\n')gl_n_token++;

    return;
}

void token_list_free(){
    if(gl_token_list =! NULL)free(gl_token_list);
    if(gl_token_search =! NULL)free(gl_token_search);
    return;
}


void token_list_print(char * token_list){
    int line_count = 0;
    printf("%d : ",line_count++);
    while(*token_list != '\0')
    {
        printf("%c",*token_list);
        if(*token_list == '\n' && token_list[1] != '\0'){
            printf("%d : ",line_count++);
        }
        token_list++;
    }
    printf("\n");
    return;
}

void token_search_init(token_search *t){
    t = calloc(1, sizeof(token_search));
    if(!file_exist()){
        printf("DEBUG = file dont exist\n");
        token_list_update(t);
    }
    else{
        printf("DEBUG = file exist, and load\n");
        token_list_load();
    }
    return;
}