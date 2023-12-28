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

/**
 * @brief regex_token_dic : Extract token price from coingecko API response.
 * 
 * @param source  coingecko API response.
 * @param dictionary The dictionary that need price update.
 * @return int 
 */
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

/**
 * @brief generate_url : build appropriate api url,
 *                       for geting price of all token 
 *                       present in the dictionary.
 * @param dictionnary    @ of dictionnary that need price updates.
 * @return char*         URL for coingecko API
 */
static char* generate_url(st_dictionary * dictionnary){
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
    free(resp.response);
}

/* List all token on coingecko */
static int token_list_query(struct memory * resp){
    char *url = "https://api.coingecko.com/api/v3/coins/list";
    ask_url(url, resp);
    return 0;
}


int token_list(){
  static struct memory api_list = {0};
  if(api_list.size == 0){
    //Query only once the list of token. durring all the program.
    token_list_query(&api_list);
  }
  printf("%.10s\n",api_list.response);
  return 0;
}