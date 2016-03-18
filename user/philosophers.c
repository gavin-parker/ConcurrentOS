#include "philosophers.h"
typedef struct{
  int flag;
  //array of forks 1 = in use, 0 = free
  int forks[5];
} table;


void startDining(char* name, int id){
//print(name,0,0,0);
print(name,0,0,0);
print(" = %d has sat down \n",id,0,0);
yield();
table * tab = getChannel();
print("%d got a channel \n",id,0,0);
while(1){
  yield();
}
}


int makePhilosopher(char* name, int id){
  int i = fork();
  if(i == 0){
  startDining(name,id);
  exit();
  return 0;
}else{
  return i;
}
}
void philosophers(){
  int ids[5];
  ids[0] = makePhilosopher("Aristotle", 0);
  yield();
  ids[1] = makePhilosopher("Kant", 1);
  yield();
  ids[2] = makePhilosopher("Marx", 2);
  yield();
  ids[3] = makePhilosopher("Russell", 3);
  yield();
  ids[4] = makePhilosopher("Spinoza", 4);
  yield();


  table diningTable;
  diningTable.flag = 0;

  for(int i=0;i<5;i++){
    print("opening channel to %d \n",i,0,0);
    openChannel(i,&diningTable);
  }


  while(1){

  }

}


void (*entry_phil)() = &philosophers;
