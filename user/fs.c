#include "fs.h"
#define fileCount 10
fd_t fdt[fileCount];
#define fileNameSize 5


//gets a usable address for a file of length len
int getAdd(int slot){
  int add = 10;
  add += slot;
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
  uint32_t a = getAdd(slot);
  disk_wr(a,text,len);
  strcpy(&fdt[slot].name[0],name);
  fdt[slot].add = a;
  fdt[slot].size = len;
  writeBootSector();
  print(fdt[slot].name,0,0,0);
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
  uint32_t add = 0;
  uint32_t size = 0;

  int a = 0;
  char* name = "five";
  for(int i=0;i< fileCount;i++){
    /*
    name = fdt[i].name;
    add = fdt[i].add;
    size = fdt[i].size;*/

    disk_wr(a,&fdt[i],sizeof(fd_t));
    a += 1;
    /*
    disk_wr(a,&add,sizeof(uint32_t));
    a += 1;
    disk_wr(a,&size,sizeof(uint32_t));
    a++;*/
  }
}
void readBootSector(){
  uint32_t add = 0;
  uint32_t size = 0;
  int a = 0;
  for(int i=0;i< fileCount;i++){
    disk_rd(a,&fdt[i],sizeof(fd_t));
    print(fdt[i].name,0,0,0);
    a++;
  }
}

void boot(){
  readBootSector();
}

void testFS(){

  char* filen1 = "cats";
  char* file1 = "I like cats\n";
  char* filen2 = "dogs";
  char* file2 = "I like dogs\n";
  char* buff[20];
  char* buff2[20];

  writeTextFile(file1,filen1);
  writeTextFile(file2,filen2);

  boot();
  //readTextFile(filen1,buff);
  //print(buff,0,0,0);
  readTextFile(filen2,buff);
  print(buff,0,0,0);
while(1){

}
}
