#include "terminal.h"

int running = 0;

//sends data to pid
void testChannel(int pid){
  sendChan(pid, 5);
}


void run(char *x){
  if(strcomp(x,"p0") == 0){
    int i = fork();
    if(i == 0){
      P0();
      exit();
    }else{
      running = i;
      yield();
    }
  }else if(strcomp(x,"p1") == 0){
    int i = fork();
    if(i == 0){
      P1();
      exit();
    }else{
      running = i;
      yield();
    }
  }else if(strcomp(x,"phil") == 0){
    int i = fork();
    if(i == 0){
      philosophers();
      exit();
    }else{
      running = i;
      while(1){
      yield();
    }
    }
  }else if(strcomp(x,"bottles") == 0){
    int i = fork();
    if(i == 0){
      bottles();
      exit();
    }else{
      running = i;
      yield();
    }
  }else if(strcomp(x,"rec") == 0){
    int i = fork();
    if(i == 0){
      rec();
      exit();
    }else{
      running = i;
      testChannel(i);
      while(1){
      yield();
    }
    }
  }else if(strcomp(x, "quit") == 0){
    kill(running);
    running = 0;
  }else if(strcomp(x, "tasks") == 0){
    print("Running: %d \n", running,0,0);
  }else{
    print(x,0,0,0);
    print(" is not a known command \n",0,0,0);
  }
  return;
}



void terminal(){
  char* x = "\r";
  char command[20] = "";
  uint32_t buffer = 0;
  print("Running the terminal\n",0,0,0);
  write(0,"--$:",4);
  while(1){
    x = read(0, x, 1);
    if((char)x[0] == '\r'){
      command[buffer] = '\0';
      write(0, "\n",1);
      if(buffer > 0){
        run(command);
        write(0, "\n",1);
      }
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
