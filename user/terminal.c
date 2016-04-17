#include "terminal.h"

int running = 0;
int mode = 0;

//sends data to pid
void testChannel(int pid){
  sendChan(pid, 0);
  sendChan(pid, 6);

  int a = getChan(pid);
  print("terminal got %d from %d",a,pid,0);
}


void run(char *x){
  if(strcomp(x,"p0") == 0){
    int i = fork();
    if(i == 0){
      P0();
      close();
    }else{
      running = i;
      if(!mode){
        while(1){
          yield();
        }
      }
    }
  }else if(strcomp(x,"p1") == 0){
    int i = fork();
    if(i == 0){
      P1();
      close();
    }else{
      running = i;
      if(!mode){
        while(1){
          yield();
        }
      }
    }
  }else if(strcomp(x,"phil") == 0){
    int i = fork();
    if(i == 0){
      philosophers();
      close();
    }else{
      running = i;
      if(!mode){
        while(1){
          yield();
        }
      }

    }
  }else if(strcomp(x,"bottles") == 0){
    int i = fork();
    if(i == 0){
      bottles();
      print("done bottles \n",0,0,0);
      close();
    }else{
      running = i;
      if(!mode){
        while(1){
          yield();
        }
      }
    }
  }else if(strcomp(x,"rec") == 0){
    int i = fork();
    if(i == 0){
      rec();
      close();
    }else{
      running = i;
      testChannel(i);
      if(!mode){
        while(1){
          yield();
        }
      }
    }
  }else if(strcomp(x,"fs") == 0){
    int i = fork();
    if(i == 0){
      testFS();
      close();
    }else{
      running = i;
      if(!mode){
        while(1){
          yield();
        }
      }

    }
  }else if(strcomp(x, "quit") == 0){
    kill(running);
    running = 0;
  }else if(strcomp(x, "&") == 0){
    mode = !mode;
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
  char x = 'a';
  char command[20] = "";
  uint32_t buffer = 0;
  print("Running the terminal\n",0,0,0);
  write(0,"--$:",4);
  while(1){
    read(0, &x, 1);
    if((char)x == '\r'){
      write(0, "\n",1);
      if(buffer > 0){
        command[buffer] = '\0';
        run(command);
        write(0, "\n",1);
      }
      buffer = 0;
      x = 'a';
      write(0,"--$:",4);
    }else{
      command[buffer] = x;
      write( 0, &x,1);
      buffer++;
    }
  }

  return;
}


void (*entry_terminal)() = &terminal;
