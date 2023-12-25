#include "file.h"

#include <stdio.h>

char gl_file_name[1024];
#define file_path (char*)"data/"
#define file_meta_name (char*)"wallet"
#define file_dict_ext (char*)".dic"
#define file_trans_ext (char*)".trans"

void file_init(char * path){
  strcpy(gl_file_name, path);
  FILE * f;
  if(fo)
}

