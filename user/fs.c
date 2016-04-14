#include "fs.h"
fd_t fdt[10];


//gets a usable address for a file of length len
int getAdd(int len){
  //TOTO
  return 0;
}

void writeTextFile(char* text, char* name){
  int slot = 0;
  for(int i=0;i< 10;i++){
    if(fdt[i].name == NULL || fdt[i].name[0] == '\0'){
      slot = i;
      break;
    }
  }
  int len = 0;
  while(text[len] != '\0'){
    len++;
  }
  uint32_t a = getAdd(len);
  disk_wr(a,text,len);
  fdt[slot].name = name;
  fdt[slot].add = a;
  fdt[slot].size = len;
}
void readTextFile(char* name, char* buffer){
  int slot = 0;
  for(int i=0;i< 10;i++){
    if(!strcomp(name,fdt[i].name)){
      slot = i;
      break;
    }
  }
  disk_rd(fdt[slot].add,buffer,fdt[slot].size);
}


void testFS(){

  print("running fs test\n",0,0,0);

  uint32_t blockNum = disk_get_block_num();
  uint32_t blockLen = disk_get_block_len();

  writeTextFile("I like cats","cats.txt");
  char* res;
  readTextFile("cats.txt",res);
  print(res,0,0,0);


while(1){

}
}
