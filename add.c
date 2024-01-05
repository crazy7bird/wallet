#include<stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
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
  time_t t;
  struct tm tm;
  int index = 0;
/*
    date.tm_year = 2023 - 1900;  // Year minus 1900
    date.tm_mon = 0;             // Month (0-11, so 0 represents January)
    date.tm_mday = 1;            // Day of the month
    date.tm_hour = 0;            // Hour
    date.tm_min = 0;             // Minute
    date.tm_sec = 0;             // Second
*/
  // DD day.
  int i;
  i= atoi(s);
  if(i <= 0 || i > 31 ) return 0;
  tm.tm_mday = i;
  for(;index<T_MAX_CHAR;index++){
    if( s[index] == '\'){
      index++;
      break;
    }
  i = atoi(&s[index]);
  if(i <= 0 ̤|| i >12) return 0;
  tm.tm_mon = i;
  for(;index<T_MAX_CHAR;index++){
    if( s[index] == '\'){
      index++;
      break;
    }
  i = atoi(&s[index]);
  if(i <= 0 || i > 99) return 0;
  // years is actual years - 1900 => actual year + 100 will cover 2000 to 3000.
  tm.tm_year = 100 + i;
   
}
#define T_MAX_CHAR 17u
#define T_MIN_CHAR 8u // DD/MM/YY or 
#define MIN_DATE 1231110000 // 1er Jan 2009, you probably have no crypto back then.
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
    printf("timeformat entry : %s \n", s );
    // Need to separate real timestamp and date formated DD/MM/YY-HH:MM:SS
    if(s_size > T_MAX_CHAR || s_size < T_MIN_CHAR){
      b_err = true;
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
      struct tm tm;
      // @todo : have to creat strptime…
      if ( strptime(s, "%Y/%m/%d-%H:%M:%S", &tm) != NULL ) t = mktime(&tm);
    }
    if(b_err){
      printf("ERROR in time format (-d) please use :\n  - Timestamp number\n  - dateformat: DD/MM/YY-HH:MM:SS\nNote: dateformat could stop at years\n");
      return 0;
    }
    printf("time - %ld \n",t);
    return t;
}

static option  options_manager(int argc, char** argv){
  int opt;
  option o = option_default;
  while( (opt = getopt(argc,argv,"d:i:t:f:")) != -1){
    switch(opt){
       case 'd' :
           o.timestamp = timeformat(optarg);
           break;
           } 
  }
  return o;
}

int main(int argc, char** argv){

  option o = options_manager(argc, argv);

  //printf("Options : %d %s %s %s %.3s %d\n",o.update_flag,o.id_flag,o.symbol_flag,o.name_flag,o.order,o.all_flag);
  //return 0;
  printf("hello from add\n");
  
}
