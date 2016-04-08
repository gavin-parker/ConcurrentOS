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
  int ids[5][2];
  int forks[5] = {1,1,1,1,1};
  print("created table at %d\n",id,0,0);
  ids[0][0] = makePhilosopher("Aristotle", 0,id);
  ids[1][0] = makePhilosopher("Kant", 1,id);
  ids[2][0] = makePhilosopher("Marx", 2,id);
  ids[3][0] = makePhilosopher("Russell", 3,id);
  ids[4][0] = makePhilosopher("Spinoza", 4,id);

  int message = 0;
  ids[0][1] = 2;
  ids[1][1] = 1;
  ids[2][1] = 1;
  ids[3][1] = 1;
  ids[4][1] = 0;
  print("all philosophers added!\n",0,0,0);
  sendChan(ids[0][0],1);
  while(1){
  for(int i=0;i<5;i++){
    if(ids[i][1] < 2){
      //needs a fork!!
      if(forks[i] && ids[(i+4)%5][1] > 0){
        //if dirty, take left fork
        ids[i][1]++;
        forks[i] = 0;
        ids[(i+4)%5][1]--;
      }else if(forks[(i+1)%5] && ids[(i+1)%5][1] > 0){
        //if dirty, take right fork
        ids[i][1]++;
        forks[(i+1)%5] = 0;
        ids[(i+1)%5][1]--;
      }
    }

  }
  for(int i=0;i<5;i++){
    if(ids[i][1] == 2){
      sendChan(ids[i][0],1);
      yield();
      sendChan(ids[i][0],2);
      forks[i] = 1;
      forks[(i+1)%5] = 1;
    }
  }

  }

}


void (*entry_phil)() = &philosophers;
