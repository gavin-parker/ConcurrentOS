#include "philosophers.h"



int makePhilosopher(char* name, int id, int table){
  int i = fork();
  if(i == 0){
    int tableId = getChan();
    int action = 0;
  while(1){
    //attempt to eat
    action = getChan();
    if(action == 1){
      print(name,0,0,0);
      print(" is eating \n",0,0,0);
    }else if(action == 2){
      print(name,0,0,0);
      print(" has stopped eating\n",0,0,0);
    }

  }
}else{
  yield();
  print("sending table id to %d \n",i,0,0);
  sendChan(i,table);
  print("sent table id to %d\n",i,0,0);
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

  int message = 0;
  int seat = ids[0];
  print("sending start to %d \n",seat,0,0);
  sendChan(seat,1);
  sendChan(ids[1],1);
  sendChan(ids[2],2);

  print("all philosophers added!\n",0,0,0);
  while(1){

  }

}


void (*entry_phil)() = &philosophers;
