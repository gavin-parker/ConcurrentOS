#include "fs.h"
#define fileCount 10
fd_t fdt[fileCount];


//gets a usable address for a file of length len
int getAdd(int len){
  int add = 140;
  for(int i=0;i < 10;i++){
    add += fdt[i].size;
  }
  //TOTO
  if(add != 140){
    add ++;
  }
  return add;
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
  len++;
  uint32_t a = getAdd(len);
  disk_wr(a,text,len);
  fdt[slot].name = name;
  fdt[slot].add = a;
  fdt[slot].size = len;
  writeBootSector();
  print("Written text file to %d\n",a,0,0);
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



void writeBootSector(){

  int a = 0;
  char* name = "fives";
  uint32_t* add = 0;
  uint32_t* size = 0;
  for(int i=0;i< fileCount;i++){
    name = fdt[i].name;
    add = &fdt[i].add;
    size = &fdt[i].size;

    disk_wr(a,name,5);
    a += 5;
    disk_wr(a,add,sizeof(uint32_t));
    a++;
    disk_wr(a,size,sizeof(uint32_t));
    a++;
  }
}
void readBootSector(){
  char name[5];
  uint32_t* add = 0;
  uint32_t* size = 0;
  int a = 0;
  for(int i=0;i< fileCount;i++){
    disk_rd(a,&name,5);
    a += 5;
    disk_rd(a,&add,sizeof(uint32_t));
    a++;
    disk_rd(a,&size,sizeof(uint32_t));
    a++;
    fdt[i].size = size;
    fdt[i].add = add;
    fdt[i].name = name;
  }
}

void boot(){
  readBootSector();
}

void testFS(){

  char* filen1 = "cats";
  char* file1 = "I like cats\n";
  char* buff[20];
  writeBootSector();
  writeTextFile(file1,filen1);

  boot();
  readTextFile(filen1,buff);
  //print(buff,0,0,0);

  /*
  char* filen1 = "cats.txt";
  char* filen2 = "dogs.txt";
  char* file1 = "I like cats\n";
  char* file2 = "I like dogs\n";
  char* buff[20];
  char* buff2[20];
  buff[0] = '\r';
  buff2[0] = '\r';
  print("running fs test\n",0,0,0);
  uint32_t blockNum = disk_get_block_num();
  uint32_t blockLen = disk_get_block_len();
  print("block len %d, block count %d \n",blockLen,blockNum,0);
  writeTextFile(file2,filen2);
  writeTextFile(file1,filen1);
  readTextFile(filen1,buff2);
  print(buff2,0,0,0);
  print("Done test\n",0,0,0);*/

while(1){

}
}
