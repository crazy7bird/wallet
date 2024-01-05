#include<stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
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

static time_t timeformat(char * s){
    printf("timeformat entry : %s \n", s );
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
