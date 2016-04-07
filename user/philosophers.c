#include "philosophers.h"



int makePhilosopher(char* name, int id){
  int i = fork();
  if(i == 0){
    print(name,0,0,0);
    print(" has sat down \n",0,0,0);
  while(1){

  }
  exit();
  return 0;
}else{
  return i;
}
}
void philosophers(){
  int ids[5];
  print("created table\n",0,0,0);
  ids[0] = makePhilosopher("Aristotle", 0);
  ids[1] = makePhilosopher("Kant", 1);
  ids[2] = makePhilosopher("Marx", 2);
  ids[3] = makePhilosopher("Russell", 3);
  ids[4] = makePhilosopher("Spinoza", 4);


  while(1){

  }

}


void (*entry_phil)() = &philosophers;
