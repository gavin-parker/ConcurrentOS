#include "rec.h"



void rec(){
  sharedMem messenger;
  messenger.flag = 0;
  putDataInSync(&messenger, 34);
  openChannel(0, &messenger);
  print("(rec)flag: %d, data: %d\n",messenger.flag,messenger.data,0);
  int i = 0;
  while(1){
    //i++;
    //putDataInSync(&messenger, i);
    //yield();
  }


}

void (*entry_rec)() = &rec;
