#include "rec.h"



void rec(){
  sharedMem messenger;
  messenger.flag = 0;
  openChannel(0, &messenger);
  print("(rec)flag: %d, data: %d\n",messenger.flag,messenger.data,0);
  int i = 0;
  while(1){
    i++;
    if(i%100000 == 0 && i > 100000){
      if(messenger.flag == 0){
      messenger.data = i;
      messenger.flag = 2;
      print("rec sent %d\n",i,0,0);
    }else{
      yield();
    }
  }
  if(i > 1000000 ){
      messenger.flag = 3;
      print("\n\n\nend rec \n",0,0,0);
      return;
  }
}
}

void (*entry_rec)() = &rec;
