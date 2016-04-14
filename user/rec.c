#include "rec.h"



void rec(){
  print("Started rec \n",0,0,0);
  int a = getChan(-1);
  print("rec done \n",0,0,0);
  print("rec got %d \n",a,0,0);
  int b = getChan(a);
  print("rec got %d from %d \n",b,a,0);
  sendChan(a,10);
  while(1){

  }
  //exit();
}

void (*entry_rec)() = &rec;
