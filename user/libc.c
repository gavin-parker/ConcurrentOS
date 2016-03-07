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
  if (x <        100) return 2;
  if (x <       1000) return 3;
  if (x <      10000) return 4;
  if (x <     100000) return 5;
  if (x <    1000000) return 6;
  if (x <   10000000) return 7;
  if (x <  100000000) return 8;
  if (x < 1000000000) return 9;
  return 10;
}

void writeDigit(int x){
switch(x){
  case 0:
  write(0, "0", 1);
  break;
  case 1:
  write(0, "1", 1);
  break;
  case 2:
  write(0, "2", 1);
  break;
  case 3:
  write(0, "3", 1);
  break;
  case 4:
  write(0, "4", 1);
  break;
  case 5:
  write(0, "5", 1);
  break;
  case 6:
  write(0, "6", 1);
  break;
  case 7:
  write(0, "7", 1);
  break;
  case 8:
  write(0, "8", 1);
  break;
  case 9:
  write(0, "9", 1);
  break;
}
return;
}



int writeInt(int fd, int x){
  int n = getLen(x);
  int y = 1;
  for(int j=1;j < n;j++){
    y *= 10;
  }

  while(y >= 1){
    writeDigit(x/y);
    x = x%y;
    y = y / 10;
  }
}
