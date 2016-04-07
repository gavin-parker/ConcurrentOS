#include "philosophers.h"



int makePhilosopher(char* name, int id, int table){
  int i = fork();
  if(i == 0){
    print(name,0,0,0);
    print(" has entered \n",0,0,0);

    int tableId = getChan();
    print("the table has pid %d \n",tableId,0,0);

  while(1){
    yield();
  }
  exit();
  return 0;
}else{
  yield();
  sendChan(i,table);
  print("sent table id \n",0,0,0);
  return i;
}
}
void philosophers(){
  int id = thisId();
  int ids[5];
  print("created table at %d\n",id,0,0);
  ids[0] = makePhilosopher("Aristotle", 0,id);
  ids[1] = makePhilosopher("Kant", 1,id);
  ids[2] = makePhilosopher("Marx", 2,id);
  ids[3] = makePhilosopher("Russell", 3,id);
  ids[4] = makePhilosopher("Spinoza", 4,id);


  while(1){
    yield();
  }

}


void (*entry_phil)() = &philosophers;
