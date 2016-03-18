#include "libc.h"

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
void yield(){
  asm volatile("svc #0     \n");
}

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
void kill(int p){
  int r;

  asm volatile( "mov r0, %0 \n"
                "svc #5     \n"
              : "=r" (r)
              : "p" (p)
              : "r0");

}
//shares an address with process with id 'pid'
void openChannel(int pid, int * add){
  int r;

  asm volatile( "mov r0, %0 \n"
                "mov r1, %1 \n"
                "svc #6     \n"
              :
              : "r" (pid), "r" (add)
              : "r0", "r1");
  yield();

}
//gets an adress from another process openchannel must have been called by
//the sender
int getChannel(){
  int r = 0;
  while(r == 0){
  asm volatile("mov %0, r0 \n"
               : "=r" (r));
  yield();
  }
  return r;
}
//waits for the flag to be set in shared address space
//then extracts data
int getDataInSync(sharedMem *mem){
  uint32_t *data = (uint32_t *) mem->data;
  print("(get)data: %d\n", data ,0,0);
  print("(get)flag: %d \n", mem->flag,0,0);
  while(1){
    if(mem->flag == 0){
      int *r = mem->data;
      print("getting %d\n",r,0,0);
      return r;
    }else{
      yield();
      //print("can't get: channel in use %d \n",  mem->flag,0,0);
    }
  }
}

void putDataInSync(sharedMem *mem, int data){
  while(1){
    if(mem->flag == 0){
      irq_unable();
      mem->flag = 1;
      mem->data = data;
      mem->flag = 2;
      print("putting %d \n", data,0,0);
      irq_enable();
      return;
    }else{
      //print("can't put: channel in use %d\n",mem->flag,0,0);
      yield();
    }
  }
}

int strcomp(char* x, char* y){
  while(*x != '\0' && *y != '\0' && *x != '\r' && *y != '\r'){
    if(*x != *y){
      return -1;
    }
    x++;
    y++;
  }
  return 1;
}
int fork(){
  int r;
  asm volatile("svc #2     \n" // dont forget to declare a new svc
                "mov %0, r0 \n"
              : "=r" (r));
  return r;
}
int clone(){
  asm volatile("svc #5     \n");
  return 0;
}
int exit(){
  asm volatile("svc #3     \n");
  return 0;
}

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
