#include "rec.h"



void rec(){
  sharedMem messenger;
  messenger.flag = 0;
  messenger.data = 34;
  openChannel(0, &messenger);
  print("flag: %d, data: %d\n",messenger.flag,messenger.data,0);
  int i = 0;
  yield();
  while(1){
    i++;
    //putDataInSync(&messenger, i);
    //yield();
  }


}

void (*entry_rec)() = &rec;
