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
int fork(){
  asm volatile("svc #2     \n");
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
