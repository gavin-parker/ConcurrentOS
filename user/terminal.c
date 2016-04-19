#include "terminal.h"

int running = 0;
int mode = 0;
char* killstr = "kill ";
char* mkdirstr = "mkdir ";
char* cdstr = "cd ";

//sends data to pid
void testChannel(int pid){
  sendChan(pid, 0);
  sendChan(pid, 6);

  int a = getChan(pid);
  print("terminal got %d from %d",a,pid,0);
}

int endProccess(char* command){
  for(int i=0;i < 5;i++){
    if(command[i] != killstr[i]){
      return 0;
    }
  }
  int p = command[5] - '0';
  print("killing process %d \n",p,0,0);
  kill(p);
  return 1;
}
void readFile(){
  char x = 'a';
  char name[20] = "";
  char text[20] = "";

  uint32_t buffer = 0;
  print("Please enter the file name\n",0,0,0);
  write(0,":",1);
  while(1){
    read(0, &x, 1);
    if((char)x == '\r'){
      write(0, "\n",1);
      if(buffer > 0){
        name[buffer] = '\0';
        break;
      }
      buffer = 0;
      x = 'a';
    }else{
      name[buffer] = x;
      write( 0, &x,1);
      buffer++;
    }
  }
  if(readTextFile(&name, text)){
    print("Opened ",0,0,0);
    print(name,0,0,0);
    print("\n",0,0,0);
    print(text,0,0,0);
    while(text[buffer] != '\0'){
      buffer++;
    }
  }else{
    print(name,0,0,0);
    print(" does not exist\n",0,0,0);
  }
}

void writeFile(){
  char x = 'a';
  char name[20] = "";
  char text[20] = "";

  uint32_t buffer = 0;
  print("Please enter the file name\n",0,0,0);
  write(0,":",1);
  while(1){
    read(0, &x, 1);
    if((char)x == '\r'){
      write(0, "\n",1);
      if(buffer > 0){
        name[buffer] = '\0';
        break;
      }
      buffer = 0;
      x = 'a';
    }else{
      name[buffer] = x;
      write( 0, &x,1);
      buffer++;
    }
  }
  if(readTextFile(&name, text)){
    print("Opened ",0,0,0);
    print(name,0,0,0);
    print("\n",0,0,0);
    print(text,0,0,0);
    buffer = 0;

    while(text[buffer] != '\0'){
      buffer++;
    }
  }else{
    print("Made ",0,0,0);
    print(name,0,0,0);
    print("\n",0,0,0);
    buffer = 0;
  }

  while(1){
    read(0, &x, 1);
    if((char)x == '\r'){
      write(0, "\n",1);
      if(buffer > 0){
        text[buffer] = '\0';
        break;
      }
      buffer = 0;
      x = 'a';
    }else{
      text[buffer] = x;
      write( 0, &x,1);
      buffer++;
    }

  }
  print("writing ",0,0,0);
  print(text,0,0,0);
  print(" \n",0,0,0);
  writeTextFile(&text,name);




}
//create a directory
int mkdir(char* command){
  for(int i=0;i < 6;i++){
    if(command[i] != mkdirstr[i]){
      return 0;
    }
  }
  createDirectory(&command[6]);
  return 1;
}
//open a directory
int cd(char* command){
  for(int i=0;i < 3;i++){
    if(command[i] != cdstr[i]){
      return 0;
    }
  }
  openDirectory(&command[3]);
  return 1;
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
  }else if(strcomp(x, "&") == 0){
    mode = !mode;
    running = 0;
  }else if(strcomp(x, "tasks") == 0){
    print("Running: %d \n", running,0,0);
  }else if(strcomp(x, "read") == 0){
    readFile();
  }else if(strcomp(x, "write") == 0){
    writeFile();
  }else if(mkdir(x)){
  }else if(cd(x)){
  }else if(strcomp(x, "ls") == 0){
    list();
  }else if(endProccess(x)){

  }else{
    print(x,0,0,0);
    print(" is not a known command \n",0,0,0);
  }
  return;
}


void terminal(){
  boot();
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
