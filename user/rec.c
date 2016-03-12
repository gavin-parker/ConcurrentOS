#include "rec.h"



void rec(){
  sharedMem messenger;
  messenger.flag = 0;
  messenger.data = 1234;
  print("sharing %d \n",&messenger,0,0);
  share(0, &messenger);
}

void (*entry_rec)() = &rec;
