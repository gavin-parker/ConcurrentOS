#include "libc.h"

//write out a string
int write( int fd, void* x, size_t n ) {
  int r;
  asm volatile( "mov r0, %1 \n"
                "mov r1, %2 \n"
                "mov r2, %3 \n"
                "svc #1     \n"
                "mov %0, r0 \n"
              : "=r" (r)
              : "r" (fd), "r" (x), "r" (n)
              : "r0", "r1", "r2" );

  return r;
}
//give control back to kernel
void yield(){
  asm volatile("svc #0     \n");
}
//read in a string
int read(int fd, void *buf, size_t nbyte) {
  int r;

  asm volatile( "mov r0, %1 \n"
                "mov r1, %2 \n"
                "mov r2, %3 \n"
                "svc #4     \n" // dont forget to declare a new svc
                "mov %0, r0 \n"
              : "=r" (r)
              : "r" (fd), "r" (buf), "r" (nbyte)
              : "r0", "r1", "r2" );

  return r;
}
//end a process with pid p
void kill(int p){
  int r;

  asm volatile( "mov r0, %0 \n"
                "svc #5     \n"
              : "=r" (r)
              : "p" (p)
              : "r0");

}
//sends data to a process, wait until data received
void sendChan(int pid, int dat){
  int r = 0;
  int flag = 0;
  int tuple[2] = {pid,dat};
  //send data
  //print("sending data to %d \n",pid,0,0);
  asm volatile( "mov r0, %0 \n"
                "svc #6     \n"
              :
              : "r" (&tuple)
              : "r0");
  //print("sent data %d to %d \n",tuple[1],tuple[0],0);
  //wait for confirmation
  //print("waiting for confirm\n",0,0,0);

  r = -1;
  while(r == -1){
    //write(0,"/",1);
    yield();
    asm volatile("mov r0, %1\n"
                 "svc #7 \n"
                 "mov %0, r0 \n"
                : "=r" (r)
                : "r" (pid)
                : "r0");
    }
return;
}

//returns the id of this process
int thisId(){
  int r;
  asm volatile("svc #8     \n" // dont forget to declare a new svc
                "mov %0, r0 \n"
              : "=r" (r));
  return r;
}
//gets data received by a channel && pid of sender
int getChan(int chan){
  int r = -1;
  int add = 0;
  int sender = -1;
  //get the data and
  //print("waiting for data\n",0,0,0);
  while(r == -1){
    //write(0,".",1);
    yield();
    asm volatile("mov r0, %1\n"
                 "svc #7 \n"
                 "mov %0, r0 \n"
                : "=r" (r)
                : "r" (chan)
                : "r0");
    //write(0,".",1);
    }
    int result = r;
  if(chan != -1){
  //print("got data %d, sender %d \n",r,sender,0);
  //send a confirmation
  int tuple[2] = {chan, 1};    //this not working??
  //print("sender: %d \n",tuple[1],0,0);
  yield();
  asm volatile( "mov r0, %0 \n"
                "svc #6     \n"
              :
              : "r" (&tuple)
              : "r0");
}
  return result;
}
//a redundant strlen function...
int strlen(char* x){
  int len=0;
  while(*x != '\0'){
    x++;
    len++;
  }
  len++;
  return len;
}
//compares two strings
int strcomp(char* x, char* y){
  while (*x != '\0') {
       if (*y == '\0') return  1;
       if (*y > *x)   return -1;
       if (*x > *y)   return  1;

       x++;
       y++;
   }

   if (*y != '\0'){
     return -1;
   }

   return 0;
}

//create an identical but seperate clone process
int fork(){
  int r;
  asm volatile("svc #2     \n" // dont forget to declare a new svc
                "mov %0, r0 \n"
              : "=r" (r));
  return r;
}
//end this process
int close(){
  asm volatile("svc #3     \n");
  return 0;
}
//print chars and ints
void print(char* x, int d1, int d2, int d3){
  int escape = 0;
  int i = 0;

  while(*x != '\0' ){
    if(*x == '%'){
      escape = 1;
    }else if(*x == 'd' && escape){
      switch(i){
        case 0:
        writeInt(0,d1);
        break;
        case 1:
        writeInt(0,d2);
        break;
        case 2:
        writeInt(0,d3);
        break;
      }
      i++;
      escape = 0;
    }else{
      write(0,x,1);
    }
    x++;
  }
  return;
}
//gets the digit length of an int
int getLen( int x){
  if (x <         10) return 1;
  if (x <        100) return 10;
  if (x <       1000) return 100;
  if (x <      10000) return 1000;
  if (x <     100000) return 10000;
  if (x <    1000000) return 100000;
  if (x <   10000000) return 1000000;
  if (x <  100000000) return 10000000;
  if (x < 1000000000) return 100000000;
  return 10;
}
//writes a digit 
void writeDigit(int x){
  PL011_putc( UART0, '0' + x );
  return;
}



int writeInt(int fd, int x){
  int y = getLen(x);

  while(y >= 1){
    int temp = x/y;
    writeDigit(temp);
    x -= (temp*y);
    y = y / 10;
  }
  return 0;
}
