#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct st_dictionary st_dictionary;
struct st_dictionary {
 int size;
 char ** token;
};
st_dictionary gl_dic;

long int file_position = 0;

static long int file_separator_set(char * path){
  FILE * f = fopen(path, "rb");
  char t1 = '\0';
  char t2 = '\0';
  t2 = fgetc(f);
  while(!feof(f)){
    t1 = t2;
    t2 = fgetc(f);
    if(t1 == '\n' && t2 == '\n') break;
  }
  // @todo : test error return from fgetpos;
  file_position = ftell(f);
  return file_position;
}

void file_separator_init(char * path){
  if(file_position == 0) file_separator_set(path);
}

long int file_separator_get(void){
  return file_position;
}

int dictionary_add_token(char * new_token){
  if(dictionary_get_ID(new_token) != -1){
     printf("dictionary_add_token : error token already exist\n");
     return -1;
  }
  gl_dic.token = realloc(gl_dic.token,gl_dic.size + 1);
  gl_dic.token[gl_dic.size] = malloc((strlen(new_token) + 1)*sizeof(char));
  strcpy(gl_dic.token[gl_dic.size], new_token);
  gl_dic.size++;
  return 0;
}

int dictionary_get_ID(char * token_id){
  for(int i = 0u ; i < gl_dic.size ; i++){
    if(strcmp(token_id, gl_dic.token[i]) == 0u) return (int)i;
  }
  return -1;
}

char * dictionary_get_token(int id){
  if(id < 0 ){
    printf("get_token error id can’t be negative\n");
    return NULL;
  }
  if(id >= gl_dic.size){
    printf("get_token error id out of range\n");
    return NULL;
  }
  return gl_dic.token[id];
}

void dictionary_print(){
  for(int i = 0; i < gl_dic.size ; i++ ){
    printf("ID : %d; Token : %s\n",i,gl_dic.token[i]);
  }
}

int dictionary_init(){
  gl_dic.size = 0;
  gl_dic.token = malloc(sizeof(int));
  gl_dic.token[0] = NULL;
  return 0;
}

int dictionary_deinit(){
  // remove all malloc ?
  for(int i = 0; i < gl_dic.size; i ++){
    free(gl_dic.token[i]);
  }
  free(gl_dic.token);
  return 0;
}

int dictionary_save(char * path){
  // Path size check used for bkp.
  if(strlen(path)>=95){
    printf("dictionary_save : error path too long");
    return -1;
  }
  // Saving old file as backup.
  char bkp_path[100];
  bkp_path[0]= '~';
  strcpy(&bkp_path[1], path);
  strcat(bkp_path,".bkp");
  rename(path, bkp_path);

  FILE * f_n = fopen(path,"wb");
  FILE * f_bkp = fopen(bkp_path, "rb");

  // writing header that is the token list.
  for(int i =0; i< gl_dic.size; i++){
    fwrite(gl_dic.token[i], strlen(gl_dic.token[i]) * sizeof(char), 1, f_n);
    fwrite("\n", sizeof(char), 1, f_n);
  }
  fwrite("\n", sizeof(char), 1, f_n);

  char t1 = '\0';
  char t2 = '\0';
  t2 = fgetc(f_bkp);
  while(!feof(f_bkp)){
     t1 = t2;
     t2 = fgetc(f_bkp);
     if(t1 == '\n' && t2 == '\n') break;
  }

  while(!feof(f_bkp)){
    char c = fgetc(f_bkp);
    if(feof(f_bkp))break;
    fputc(c, f_n);
  }
  fclose(f_bkp);
  fclose(f_n);
  //int remove(f_bkp);
  return 0;
}

int dictionary_load(char * path){
  FILE * f = fopen(path,"rb");
  char LINE[100];
  do{
    fgets(LINE, 100, f);
    LINE[strlen(LINE) - 1u] = '\0'; //replace \n by \0 
    if(strlen(LINE) == 0) break;
    dictionary_add_token(LINE);
  }while(!feof(f));
  return 0;
}
/*
int main(void){
  dictionary_init();
  
  dictionary_add_token("halala");
  dictionary_add_token("hilili");
  dictionary_add_token("hoilili");
  dictionary_add_token("FutureProof");
  dictionary_get_ID("hilili");
  printf("dgt : %s \n",dictionary_get_token(1));
  printf("dgt - fault : %s \n",dictionary_get_token(999));
  printf("dgi : %d\n", dictionary_get_ID("hilili"));
  printf("dgi - fault : %d\n", dictionary_get_ID("hama"));
  dictionary_print();
  dictionary_save("data.bin");
  
  dictionary_load("data.bin");
  printf("%ld\n",file_separator_set("data.bin"));
  dictionary_print();
  dictionary_deinit();

}
*/
