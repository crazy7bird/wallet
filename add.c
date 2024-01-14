#include<stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "inc/dictionary.h"
#include "inc/transaction.h"

#define T_MAX_CHAR 17u
#define T_MIN_CHAR 8u // DD/MM/YY or 
#define MIN_DATE 1231110000 // 1er Jan 2009, you probably have no crypto back then.
/**
* Contain options from args
*/
struct option{
  time_t timestamp;  // -d for date
  bool d_flag;
  uint8_t ID;   // -i
  bool id_flag;
  double token_flag; // -t for token
  bool t_flag;
  double fiat_flag;  // -f for fiat
  bool f_flag;
}option_default = {0,false,255,false,0,false,0,false};
typedef struct option option;


/**
 * @brief help when -h is given.
 * 
 */
static void print_help(void){
  printf("add prog help user adding a transaction\n");
  printf("-d for setting the date (use NOW for setting at current time)\n");
}
/**
* @brief convert str format DD/MM/YY-HH:mm:ss to timestamp.
* @return timestamp or 0 if error.
*/ 
static time_t str2time(char *s){
  // Init all to 0.
  struct tm tm;
  tm.tm_year = 0;            // Year minus 1900
  tm.tm_mon = 0;             // Month (0-11, so 0 represents January)
  tm.tm_mday = 0;            // Day of the month
  tm.tm_hour = 0;            // Hour
  tm.tm_min = 0;             // Minute
  tm.tm_sec = 0;             // Second

  // DD day.
  int i;
  i= atoi(s);
  if(i <= 0 || i > 31 ) return 0;
  tm.tm_mday = i;

  while(*s != '/' && *s != '\0') s++;
  if(*s == '\0') return 0;
  s++;
  //MM January is 0;
  i = atoi(s) -1;
  if(i <= 0  || i >12) return 0;
  tm.tm_mon = i;

  while(*s != '/' && *s != '\0') s++;
  if(*s == '\0') return 0;
  s++;
  // YY or YYYY Years is from 1900.
  i = atoi(s);
  if(i <= 0 || i > 3000) return 0;
  if(i <= 99) tm.tm_year = (2000 + i) - 1900;
  else tm.tm_year = i - 1900;

  while(*s != '-' && *s != '\0') s++;
  if(*s == '\0') return mktime(&tm);
  s++;
  // HH 0-23H
  i = atoi(s);
  if(i <= 0 || i > 23 ) return 0;
  tm.tm_hour = i;

  while(*s != ':' && *s != '\0') s++;
  if(*s == '\0') return mktime(&tm);
  s++;
  // mm 0-59 min
  i = atoi(s);
  if(i <= 0 || i > 59 ) return 0;
  tm.tm_min = i;

  while(*s != ':' && *s != '\0') s++;
  if(*s == '\0') return mktime(&tm);
  s++;
  i = atoi(s);
  if(i <= 0 || i > 59 ) return 0;
  tm.tm_sec = i;
  
  return mktime(&tm);
}


/**
 * @brief Convert a string to a timestamp.
 *  The string could be DD/MM/YY-HH:MM:SS format or a real timestamp (64 bit number) but in str.
 * 
 * @param s the string with time information.
 * @return time_t timestamp decoded.
 */
static time_t timeformat(char * s){
    size_t s_size = strlen(s);
    time_t t;

    if(strcmp(s, "NOW") == 0){
      return time(NULL);
    }

    // Need to separate real timestamp and date formated DD/MM/YY-HH:MM:SS
    if(s_size > T_MAX_CHAR || s_size < T_MIN_CHAR){
      return 0;
    }
    // Determine if it’s a Timestamp or a string.
    bool s_is_just_a_bunch_of_digits = true;
    for(int i=0; i<s_size && s_is_just_a_bunch_of_digits; i++){
      s_is_just_a_bunch_of_digits = isdigit(s[i]);
    }
    if(s_is_just_a_bunch_of_digits){
      t = atoll(s);
    }
    else{
      // @todo : have to creat strptime…
      t = str2time(s);
    }
    return t;
}

static void  options_manager(int argc, char** argv, option *o){
  int opt;
  optind = 1; //Reset the index for getopt
  while( (opt = getopt(argc,argv,"d:i:t:f:sh")) != -1){
    switch(opt){
      case 'h' :
        print_help();
        break;
      case 's' :
        printf("Dictionary contains : \n");
        dictionary_print();
        break;
      case 'd' :
        o->timestamp = timeformat(optarg);
        if(o->timestamp == 0){
          printf("ERROR in time format\n");
        }
        else{
          o->d_flag = true;
        } 
        break;
      case 'i' :
        o->ID = dictionary_get_ID(optarg);
        if(o->ID == 255){
          printf("ERROR bad ID\n");
        }
        else {
          o->id_flag = true;
        }
        break;
      case 't' :
        {
        char * index = optarg;
        while(*index != '\0'){
          if(isdigit(*index)|| *index == '.'){
            index++;
          }
          else{
            printf("ERROR token should be a number\n");
            break;
          }
        }
        o->token_flag = atof(optarg);
        o->t_flag = true;
        break;
        }
      case 'f':
      {
        char * index = optarg;
        while(*index != '\0'){
          if(isdigit(*index)|| *index == '.'){
            index++;
          }
          else{
            printf("ERROR fiat should be a number\n");
            break;
          }
        }
        o->fiat_flag = atof(optarg);
        o->f_flag = true;
        break;
      } 
    } 
  }
}

int main(int argc, char** argv){

  // Init dictionary
  dictionary_load();
  option o = option_default;
  options_manager(argc, argv,&o);
  while(1){
    // to do validations 
    printf("Would you like to store this transaction : \n");
    {
      char date[25] = {'\0'};
      struct tm * tm;
      tm = localtime(&o.timestamp);
      tm->tm_isdst = 0; // Else it remove 1H for daysaving time.
      strftime(date,25, "%d/%m/%y-%H:%M:%S",tm);
      printf("  (-d)Timestamp\t: %ld (%s)\n", o.timestamp, date);
      st_token * t = dictionary_get_token(o.ID);
      if(t != NULL){
        printf("  (-i)Token \t: %s, %s, %s\n",t->token_id,t->token_symbol, t->token_name);
      }
      else{
        printf("  (-i)Token \t: unknow (-s to show dictionary entries)\n");
      }
        printf("  (-t)Amount \t: %lf ", o.token_flag);
        if(t != NULL){
          printf("%s\n", t->token_symbol);
        }
        else{
          printf("unknow\n");
        }
        printf("  (-f)Fiat   \t: %lf €\n", o.fiat_flag);
    }
    char resp[100]={'\0'};
    printf("You can correct (using -X arg) or use Yes (y) No (n) to saves or exit:\n");
    //ScanF not working for spaced string.
    fgets(resp, 100, stdin);
    resp[strlen(resp) - 1] = '\0'; // remove extra \n
    //scanf("%100s", resp);
    if(resp[0] == 'N' || resp[0] == 'n') return 0;
    if(resp[0] == 'Y' || resp[0] == 'y'){
      if( o.d_flag &&
          o.f_flag &&
          o.id_flag &&
          o.t_flag ){
            st_transaction t = {o.d_flag, o.id_flag, o.t_flag, o.f_flag};
            transaction_save(t);
            return 0;
          }
      printf("Transactions datas are not corrects\n");
    }
    // Construct a new argc argv.
    #define MAX_ARGS 10 //Arbitrairy value.
    int c = 1; // Argv 0 contain progname.
    char *v[MAX_ARGS];// = {"add.c"};
    char * ptr = strtok(resp, " ");
    while (ptr && c < MAX_ARGS-1){
      v[c] = ptr;
      c = c+1;
      ptr = strtok(0," ");
    }
    v[c] = 0;
    options_manager(c,v,&o);
  }

  printf("bye from add\n");
  return 0;
  
}
