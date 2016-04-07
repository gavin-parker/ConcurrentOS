#include "rec.h"



void rec(){
  print("Started rec \n",0,0,0);
  int a = getChan();
  print("rec done \n",0,0,0);
  print("rec got %d \n",a,0,0);
  exit();
}

void (*entry_rec)() = &rec;
