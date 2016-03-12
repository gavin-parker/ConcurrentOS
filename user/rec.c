#include "rec.h"



void rec(){
  sharedMem messenger;
  messenger.flag = 0;
  share(0,5);
}

void (*entry_rec)() = &rec;
