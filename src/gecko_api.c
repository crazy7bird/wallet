#include <stdlib.h> /* for realloc */
#include <string.h> /* for memcpy */
#include <curl/curl.h> /* for http.get request */
#include <stdio.h>     /* printf */
#include <regex.h>    /* data treatment */
#include "gecko_api.h"



/* Used for calback on http.get response */ 
struct memory {
  char *response;
  size_t size;
};

/* callback for http.get response */
static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;
 
  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory! */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}

/** make_url :
*  @brief : generate desired url with a token_list,
*           this url with desired token will reduce
*           coingecko response.
*  @param : int token_num, numbers of token in token_list.
*  @param : char **token_list, char array of token.
*  @return : char * on the url created.
*/
char* make_url(int token_num, char **token_list){
  char * finalUrl = 0;
  char * finalUrl_tail = 0;
  size_t finalSize;
  const char * url_b = "https://api.coingecko.com/api/v3/simple/price?vs_currencies=eur&ids=";
  const char * link = "%2C";
  finalSize = strlen(url_b);
  finalSize += token_num * strlen(link);
  for(int i = 0; i < token_num; i++){
    finalSize += strlen(token_list[i]); 
  } 
  finalSize ++; /* \0 */
  finalUrl = (char *) malloc(finalSize * sizeof(char)); 
  finalUrl_tail = strcpy(finalUrl, url_b);
  
  for(int i = 0; i < token_num; i++){
    finalUrl_tail = strcat(finalUrl_tail, token_list[i]);
    finalUrl_tail = strcat(finalUrl_tail, link);
  }
 
 return finalUrl;
  
}
/* @end : make_url */

/**  ask_url
*    @brief : sent http.get request
*             with coin listed in argv (size argc)
*    @param : int token_num, number of token.
*    @param : char **token_list, tab contain tokens.
*    @param : char * ret_data, pointer where response will be writen.
*
*    @return : int 0 if all good.
*/
int ask_url(char* url, struct memory* ret_data)
{
  CURLcode res;
  CURL *curl = curl_easy_init();
  if(!curl){
    fprintf(stderr, "Failed to init curl");
    return 1;
  }
  /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)ret_data);
  curl_easy_setopt(curl, CURLOPT_URL, url); 
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "barely - human 1.02");
  /* send a request */
  res = curl_easy_perform(curl);
  /* remember to free the buffer */
  curl_easy_cleanup(curl);
  return 0;
}
/* @end ask_coingecko */

/** regex_token 
*  @brief : get data and found token + value in it.
*  @return ???
*  @help : https://gist.github.com/ianmackinnon/3294587
*/
int regex_token (char* source, int num_token, struct token_api * t_list)
{
  //char * regexString =  "\"id\":\"([^\"]*)\"[^}]*current_price\".([^,]*)";
  char * regexString = "\"([^\"]*)\":[^:]*:([^}]*)}";
  size_t maxGroups = 3*num_token;
  
  regex_t regexCompiled;
  regmatch_t groupArray[maxGroups];
  char * cursor;

 if( 0!= regcomp(&regexCompiled, regexString, REG_EXTENDED))return 1;

  cursor = source;
  int t_index = 0;
  while ( regexec(&regexCompiled, cursor, maxGroups, groupArray, 0) == 0)
    {
      // the second element is the number.
      t_list[t_index].name  = malloc((size_t) (groupArray[1].rm_eo - groupArray[1].rm_so) * sizeof(char)); 
      strncpy(t_list[t_index].name, &cursor[groupArray[1].rm_so],(size_t) (groupArray[1].rm_eo - groupArray[1].rm_so) );
       
      t_list[t_index].price = strtod(&cursor[groupArray[2].rm_so],NULL);

      cursor += groupArray[0].rm_eo;
      t_index ++;
    }

  regfree(&regexCompiled);
  return 0;
}
/* @end : regex_token*/

/** get_token_price
*  return token price.
*/
struct token_api * get_token_price(int num, char **list){
  struct token_api *  t_list = malloc((num +1)* sizeof(struct token_api));
  t_list[num].name = NULL;
  t_list[num].price = 0.0;
  struct memory chunk = {0};
  char* url = make_url(num,list);
  ask_url(url, &chunk);
  regex_token(chunk.response, num, t_list);
  free(url);
  free(chunk.response);
  return t_list;
}

void print_token_list (struct token_api * t_list){
 int t_index =0;
 while(t_list[t_index].name != NULL){
  printf("Token : %s = %lf\n", t_list[t_index].name, t_list[t_index].price);
  t_index++;
 }
}

/** free_token. 
*   @brief : use to clear a token_list.
*/
int free_token (struct token_api * t_list){
 int t_index = 0;
 while(t_list[t_index].name != NULL){
  free(t_list[t_index].name);
  t_index++;
 }
 free(t_list);
 return 0;
}

/*
int main(int argc, char **argv)
{
  struct token_api * token_list = get_token_price(argc-1, &argv[1]);
  print_token_list(token_list);
  free_token(token_list);
  return 0;
}
*/

int gecko_api(int argc, char **argv)
{
  struct token_api * token_list = get_token_price(argc-1, &argv[1]);
  print_token_list(token_list);
  free_token(token_list);
  return 0;
}
/*************************************************************************************************************/
int regex_token_dic (char* source, st_dictionary * dictionary)
{
  //char * regexString =  "\"id\":\"([^\"]*)\"[^}]*current_price\".([^,]*)";
  char * regexString = "\"([^\"]*)\":[^:]*:([^}]*)}";
  size_t maxGroups = 3*dictionary->size;
  
  regex_t regexCompiled;
  regmatch_t groupArray[maxGroups];
  char * cursor;

 if( 0!= regcomp(&regexCompiled, regexString, REG_EXTENDED))return 1;

  cursor = source;
  int t_index = 0;
  while ( regexec(&regexCompiled, cursor, maxGroups, groupArray, 0) == 0)
    {
      char buffer[DICT_MAX_CHAR] = {0};
      // the second element is the number.
      //t_list[t_index].name  = malloc((size_t) (groupArray[1].rm_eo - groupArray[1].rm_so) * sizeof(char)); 
      strncpy(buffer, &cursor[groupArray[1].rm_so],(size_t) (groupArray[1].rm_eo - groupArray[1].rm_so) );
      dictionary->entry[dictionary_get_ID(buffer)].value = strtod(&cursor[groupArray[2].rm_so],NULL);
      //t_list[t_index].price = strtod(&cursor[groupArray[2].rm_so],NULL);

      cursor += groupArray[0].rm_eo;
      t_index ++;
    }

  regfree(&regexCompiled);
  return 0;
}
/* @end : regex_token*/

char* generate_url(st_dictionary * dictionnary){
  char * finalUrl = 0;
  char * finalUrl_tail = 0;
  size_t finalSize;
  const char * url_b = "https://api.coingecko.com/api/v3/simple/price?vs_currencies=eur&ids=";
  const char * link = "%2C";
  finalSize = strlen(url_b);
  finalSize += dictionnary->size * strlen(link);
  for(int i = 0; i < dictionnary->size; i++){
    finalSize += strlen(dictionnary->entry[i].token_id); 
  } 
  finalSize ++; /* \0 */
  finalUrl = (char *) malloc(finalSize * sizeof(char)); 
  finalUrl_tail = strcpy(finalUrl, url_b);
  
  for(int i = 0; i < dictionnary->size; i++){
    finalUrl_tail = strcat(finalUrl_tail, dictionnary->entry[i].token_id);
    finalUrl_tail = strcat(finalUrl_tail, link);
  }
 return finalUrl;
  
}
int price_update(st_dictionary * dictionary){
    struct memory resp = {0};
    char * url = generate_url(dictionary);
    ask_url(url, &resp);
    regex_token_dic(resp.response, dictionary);
}