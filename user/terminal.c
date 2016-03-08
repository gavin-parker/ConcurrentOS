#include "terminal.h"

void run(char *x){
  if(strcmp(x,"p0") != -1){
    print("running p0\n",0,0,0);
  }else if(strcmp(x,"p1") != -1){
    print("running p1\n",0,0,0);
  }else{
    print(x,0,0,0);
    print(" is not a known command \n",0,0,0);
  }
return;
}
void nothing(){

}


void terminal(){
  char* x = "";
  char command[20] = "";
  uint32_t buffer = 0;
  print("Running the terminal\n",0,0,0);
  write(0,"--$:",4);
  while(1){
    x = read(0, x, 1);
    nothing();
    if((char)x[0] == '\r'){
      command[buffer] = '\0';
      run(command);
      write(0, "\n",1);
      buffer = 0;
      write(0,"--$:",4);

    }else{
      command[buffer] = *x;
      write( 0, x,1);
      buffer++;
    }

  }

return;
}


void (*entry_terminal)() = &terminal;
