#include "terminal.h"

void run(){
  print("running\n",0,0,0);
return;
}


void terminal(){
  char* x = "";
  char command[20];
  uint32_t buffer = 0;
  while(1){
    //print("Running the terminal\n",0,0,0);
    read(0, x, 1);
    if(*x == 'a'){
      run();
    }else{
      write( 0, x,1);
    }

  }

return;
}


void (*entry_terminal)() = &terminal;
