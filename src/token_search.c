#include "token_search.h"
#include "gecko_api.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief token_list is global var that contain list of all token from coingecko.
 * 
 */
char * gl_token_list = NULL;
int gl_n_token = 0;
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
    return number_token;
}

/**
 * @brief Parse str till \n then return the next char
 * 
 * @param buffer 
 * @return char* 
 */
static char* next_line(char* buffer){
    while(*buffer != '\n' && *buffer != '\0')buffer++;
    if(*buffer == '\0' || buffer[1] == '\0') return NULL;
    return buffer++;
}
/**
 * @brief like next_line but for comma.
 * Will also return on \n
 * 
 * @param buffer 
 * @return char* 
 */
static char* next_comma(char* buffer){
    while(*buffer != '\n' && *buffer != ',' && *buffer != '\0')buffer++;
    if(*buffer == '\0' || buffer[1] == '\0') return NULL;
    return buffer++;
}


char * token_search_by_id(char* id){
    char * buffer = NULL;
}

void token_list_free(){
    free(gl_token_list);
    return;
}

void token_list_print(){
    printf("%s\n", gl_token_list);
    printf("%d\n",gl_n_token);
    return;
}

void token_list_init(){
    gl_token_list = token_list();
    gl_n_token = line_converter();
    return;
}