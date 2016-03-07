#include "terminal.h"

void terminal(){

  while(1){
    print("Running the terminal\n",0,0,0);
  }

return;
}
void (*entry_terminal)() = &terminal;
