#include<stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "inc/dictionary.h"

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
}option_default = {0,false,0,false,0,false,0,false};
typedef struct option option;

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
    bool b_err = false;
    size_t s_size = strlen(s);
    time_t t;

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

static option  options_manager(int argc, char** argv){
  int opt;
  option o = option_default;
  bool b_err = false;
  while( (opt = getopt(argc,argv,"d:i:t:f:")) != -1){
    switch(opt){
      case 'd' :
        o.timestamp = timeformat(optarg);
        printf("[DEBUG]Time decoded : %ld\n", o.timestamp);
        if(o.timestamp == 0){
          b_err = true;
          printf("Error in time format\n");
        }
          o.d_flag = true;
          break;
      case 'i' :
        o.ID = dictionary_get_ID(optarg);
        printf("[DEBUG]ID obtained from %s : %d\n",optarg,o.ID);
        if(o.ID == 255){
          b_err = true;
          printf("Error bad ID found\n");
          printf("dictionnary contains :\n");
          dictionary_print();
        }
        o.id_flag = true;
        break;
      case 't' :
        {
        char * index = optarg;
        while(*index != '\0'){
          if(isdigit(*index)|| *index == '.'){
            index++;
          }
          else{
            b_err = true;
            printf("Error token should be a number\n");
            break;
          }
        }
        o.token_flag = atof(optarg);
        printf("Token amount : %.8lf\n", o.token_flag);
        o.t_flag = true;
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
            b_err = true;
            printf("Error fiat should be a number\n");
            break;
          }
        }
        o.fiat_flag = atof(optarg);
        printf("Fiat amount : %.2lf\n", o.fiat_flag);
        o.f_flag = true;
        break;
      } 
    } 
  }
  if(b_err){
    printf("Fatal error - exit\n");
    exit(1);
  }
  return o;
}

int main(int argc, char** argv){

  // Init dictionary
  dictionary_load();

  option o = options_manager(argc, argv);

  //printf("Options : %d %s %s %s %.3s %d\n",o.update_flag,o.id_flag,o.symbol_flag,o.name_flag,o.order,o.all_flag);
  //return 0;
  printf("hello from add\n");
  
}
