#include "terminal.h"

int running = 1;

void run(char *x){
  if(strcomp(x,"p0") != -1){
    int i = fork();
    if(i == 0){
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
}else if(strcomp(x, "quit") != -1){
  kill(running);
  running = -1;
}else{
    print(x,0,0,0);
    print(" is not a known command \n",0,0,0);
  }
return;
}



void terminal(){
  char* x = "";
  char command[20] = "";
  uint32_t buffer = 0;
  print("Running the terminal\n",0,0,0);
  write(0,"--$:",4);
  while(1){
    x = read(0, x, 1);
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
