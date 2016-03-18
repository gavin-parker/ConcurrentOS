#include "terminal.h"

int running = 0;

//recieves and prints out changes to shared memory
void testChannel(){
  sharedMem * dat = getChannel();
  print("(term)flag: %d, data: %d\n",dat->flag,dat->data,0);
  while(1){
    if(dat->flag == 2){
    print("terminal recieved %d\n",dat->data,0,0);
    dat->flag == 0;
  }else if(dat->flag == 3){
    print("\n communication closed",0,0,0);
    return;
  }else{
    yield();
  }
  }
}


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
  }else if(strcomp(x,"philosophers") != -1){
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
  }else if(strcomp(x,"bottles") != -1){
    int i = fork();
    if(i == 0){
      bottles();
      exit();
    }else{
      running = i;
      yield();
    }
  }else if(strcomp(x,"rec") != -1){
    int i = fork();
    if(i == 0){
      rec();
      exit();
    }else{
      running = i;
      yield();
      testChannel();
    }
  }else if(strcomp(x, "quit") != -1){
    kill(running);
    running = 0;
  }else if(strcomp(x, "tasks") != -1){
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
