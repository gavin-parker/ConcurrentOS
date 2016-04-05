#include "rec.h"



void rec(){
  print("Started rec \n",0,0,0);
  getChan();
  print("rec done \n",0,0,0);
  //print("rec got %d \n",a,0,0);
  while(1){
    
  }
}

void (*entry_rec)() = &rec;
