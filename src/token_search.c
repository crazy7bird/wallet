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
/**
 * @brief Convert the Json url : [{}]into \\n
 * 
 * @return int 
 */
int line_converter_old(){

    int len = strlen(gl_token_list);
    int number_token = 0;
    int y = 0;
    for(int i =0; i<len ; i++){
        char c = gl_token_list[i];
        if(c == '{' ||
           c == '[' ||
           c == ']') continue;
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
/* id,symbol,name */
int line_converter(){
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

void free_token_list(){
    free(gl_token_list);
}

char* test_token_list(){
    gl_token_list = token_list();
    //printf("test_token_list : %p\n", gl_token_list);
    //printf("@token_search : %.10s",gl_token_list);
    int l = line_converter();
    printf("%s\n", gl_token_list);
    printf("%d\n",l);
    return gl_token_list;
}