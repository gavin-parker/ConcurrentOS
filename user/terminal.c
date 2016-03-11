#include "terminal.h"

int running = -1;
void run(char *x){
  if(strcomp(x,"p0") != -1){
    int i = fork();
    if(i == 0){
      running = i;
    P0();
    exit();
  }else{
    running = i;
    yield();
  }
  }else if(strcomp(x,"p1") != -1){
    int i = fork();
    if(i == 0){
    P1();
    exit();
  }else{
    running = i;
    yield();
  }
  }else if(strcomp(x,"bottles") != -1){
    int i = fork();
    if(i == 0){
    bottles();
    exit();
  }else{
    running = i;
    yield();
  }
}else if(strcomp(x, "q") != -1){
  if(running > 0){
  print("killing process %d \n",running,0,0);
  kill(running);
  running = -1;
}
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
      write(0, "\n",1);
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
