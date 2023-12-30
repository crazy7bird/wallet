#include "token_search.h"

/**
 * @brief token_list is global var that contain list of all token from coingecko.
 * 
 */
char * gl_token_list = NULL;
/**
 * @brief Convert the Json url : [{}]into \\n
 * 
 * @param url_resp 
 * @return int 
 */
int line_converter(char * url_resp){

    int len = strlen(url_resp);
    //malloc will be a little oversized.
    gl_token_list = malloc(len*sizeof(char));
    int y = 0;
    for(int i =0; i<len ; i++){
        char c = url_resp[i];
        if(c != '{' && 
           c != '[' &&
           c != ']') continue;
        if(c == '}') c = '\n';
        gl_token_list[y]=url_resp[i];
        y++;
    }
}