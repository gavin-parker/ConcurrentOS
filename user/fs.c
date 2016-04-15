#include "fs.h"
fd_t fdt[10];


//gets a usable address for a file of length len
int getAdd(int len){
  //TOTO
  return 0;
}

void writeTextFile(char* text, char* name){
  print("Writing text file \n",0,0,0);
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
  len++;
  uint32_t a = getAdd(len);
  disk_wr(a,text,len);
  fdt[slot].name = name;
  fdt[slot].add = a;
  fdt[slot].size = len;
  print("Written text file \n",0,0,0);
}
void readTextFile(char* name, char* buffer){
  print("Reading text file \n",0,0,0);

  int slot = 0;
  for(int i=0;i< 10;i++){
    if(!strcomp(name,fdt[i].name)){
      slot = i;
      break;
    }
  }
  disk_rd(fdt[slot].add,buffer,fdt[slot].size);
  print("Read text file from address %d \n",fdt[slot].add,0,0);

}


void testFS(){

  print("running fs test\n",0,0,0);
  uint32_t blockNum = disk_get_block_num();
  uint32_t blockLen = disk_get_block_len();
  writeTextFile("I like cats","cats.txt");
  char* buff = "\r";
  readTextFile("cats.txt",buff);
  print(buff,0,0,0);


while(1){

}
}
