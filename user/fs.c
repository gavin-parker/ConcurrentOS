#include "fs.h"
#include "bottles.h"
#define fileCount 10
fd_t fdt[fileCount];
#define fileNameSize 5
int directoryAddress = 0;

/*
FILESYSTEM OVERVIEW:
Storage consists of a 'boot sector' that is loaded on startup and maps file names
to their contents addresses, and directories to their own 'sectors' that work in
the same way. When files are created, they are added to the current sector.
Known issues:
  text files can be too big and overwrite the next file
  only 1 new directoryu per directory (sorry)


*/


//gets a usable address for a file of length len
int getAdd(int slot){
  int add = 10;
  add += slot;
  return add;
}

//list all the file names in this directory
void list(){
  for(int i=0;i<10;i++){
    if(fdt[i].size > 0){
    print(fdt[i].name,0,0,0);
    print("\n",0,0,0);
  }
  }
}
//write a string into storage
void writeTextFile(char* text, char* name){
  int slot = -1;
  for(int i=0;i< 10;i++){
    if(!strcomp(name,fdt[i].name)){
      slot = i;
      break;
    }
  }
  if(slot == -1){
  for(int i=0;i< 10;i++){
    if(fdt[i].name == NULL || fdt[i].name[0] == '\0'){
      slot = i;
      break;
    }
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

//read a string from storage
int readTextFile(char* name, char* buff){
  print("Reading text file \n",0,0,0);
  int slot = -1;
  for(int i=0;i< 10;i++){
    if(!strcomp(name,fdt[i].name)){
      slot = i;
      break;
    }
  }
  if(slot < 0){
    return 0;
  }
  disk_rd(fdt[slot].add,buff,fdt[slot].size);
  print("Read text file from address %d \n",fdt[slot].add,0,0);
  return 1;
  }
uint32_t getDirectoryAddress(){
  return directoryAddress + 20;
}

//open and load sector with given name
void openDirectory(char* name){
  int slot = -1;
  for(int i=0;i< 10;i++){
    if(!strcomp(name,fdt[i].name)){
      slot = i;
      break;
    }
  }
  if(slot>=0){
    directoryAddress = fdt[slot].add;

  readBootSector();
  print("opened ",0,0,0);
  print(name,0,0,0);
  print("/ \n",0,0,0);
}
}
//create a new sector and point to it in current sector
void createDirectory(char* name){
  int slot = -1;
  for(int i=0;i< 10;i++){
    if(!strcomp(name,fdt[i].name)){
      slot = i;
      break;
    }
  }
  if(slot == -1){
  for(int i=0;i< 10;i++){
    if(fdt[i].name == NULL || fdt[i].name[0] == '\0'){
      slot = i;
      break;
    }
  }
}
strcpy(&fdt[slot].name[0],name);
fdt[slot].size = 4;
fdt[slot].add = getDirectoryAddress();
fd_t directory[fileCount];
disk_wr(fdt[slot].add,&directory,sizeof(directory));
print("created directory\n",0,0,0);
writeBootSector();
}
//save the current sector to storage
void writeBootSector(){
  uint32_t add = 0;
  uint32_t size = 0;

  int a = directoryAddress;
  char* name = "five";
  for(int i=0;i< fileCount;i++){
    disk_wr(a,&fdt[i],sizeof(fd_t));
    a += 1;

  }
}
//save a program state
//not implemented
void save(void (*func)(void) ){
}

//reads the boot sector from storage
void readBootSector(){
  uint32_t add = 0;
  uint32_t size = 0;
  int a = directoryAddress;
  for(int i=0;i< fileCount;i++){
    disk_rd(a,&fdt[i],sizeof(fd_t));
    a++;
  }
}

void boot(){
  readBootSector();
}
//a test program to write and read a file
void testFS(){

  char* filen1 = "cats";
  char* file1 = "I like cats\n";
  char* filen2 = "dogs";
  char* file2 = "I like dogs\n";
  char buff[20] = {NULL};

  writeTextFile(file1,filen1);
  writeTextFile(file2,filen2);

  boot();
  readTextFile(filen1, buff);
  print(buff,0,0,0);

while(1){

}
}
