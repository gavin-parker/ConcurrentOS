#include "philosophers.h"




int getFork(int seat){
  return 0;
}

int makePhilosopher(char* name,int seat,int table){
  int i = fork();
  if(i == 0){
    int tableId = getChan(-1);
    int pid = getChan(tableId);
    int lneighbour = getChan(tableId);
    int rneighbour = getChan(tableId);
    int action = 0;
    print("Philosopher id:%d has sat down next to id:%d & %d\n",pid,lneighbour,rneighbour);
    int lFork = 0;
    int rFork = 0;
    int eating = 0;
    //head of the table starts with 2 forks
    if(seat == 0){
      lFork = 1;
      rFork = 1;
    }else if(seat < 4){
      lFork = 1;
    }
    int eatcount = 0;
  while(1){
    //if eating ignore everything for a bit
    if(eating){
      print("%d is eating\n",pid,0,0);
      eatcount++;
      if(eatcount >= 5){
        eatcount = 0;
        eating = 0;
        rFork = 2;
        lFork = 2;
        print("%d has stopped eating\n",pid,0,0);
      }
    }else{
    //if you have 2 clean forks, start eating
    if(lFork == 1 && rFork == 1){
      eating = 1;
    }
    //if a fork is dirty, clean it and send to neighbour
    if(lFork == 2){
      sendChan(lneighbour,1);
      lFork = 0;
    }
    if(rFork == 2){
      sendChan(rneighbour,1);
      rFork = 0;
    }

    //if no fork, ask neighbour
    if(rFork == 0){
      rFork = getChan(rneighbour);
      print("%d has got a clean fork from %d \n",pid,rneighbour,0);
    }else if(lFork == 0){
      lFork = getChan(lneighbour);
      print("%d has got a clean fork from %d \n",pid,lneighbour,0);

    }
  }

    yield();

  }
}else{
  yield();
  sendChan(i,table);
  print("sent table id to %d\n",i,0,0);
  sendChan(i,i);
  return i;
}
}



void philosophers(){

  int id = thisId();
  int ids[5];
  int forks[5] = {1,1,1,1,1};
  print("created table at %d\n",id,0,0);
  ids[0] = makePhilosopher("Aristotle",0,id);
  ids[1] = makePhilosopher("Kant",1,id);
  ids[2] = makePhilosopher("Marx",2,id);
  ids[3] = makePhilosopher("Russell",3,id);
  ids[4] = makePhilosopher("Spinoza",4,id);
  print("all philosophers added!\n",0,0,0);
  //send Neighbours
  for(int i=0;i < 5;i++){
    sendChan(ids[i], ids[(i+1)%5]);
  }
  for(int i=0;i < 5;i++){
    sendChan(ids[i], ids[(i+4)%5]);
  }

  while(1){

  }

}


void (*entry_phil)() = &philosophers;
