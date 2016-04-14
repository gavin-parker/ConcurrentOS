#ifndef __LIBC_H
#define __LIBC_H

#include <stddef.h>
#include <stdint.h>
#include "PL011.h"
#include "fs.h"

//structure to share data between processes
//only use handler methods to get data !!
typedef struct {
  //0 = not in use
  //1 = in use
  //2 = not in use, just changed
  int  flag;
  int  data;
} sharedMem;

int read(int fd, void *x, size_t n);
int write( int fd, void* x, size_t n );
int writeInt( int fd, int x);
int getLen( int x);
int fork();
int close();
void yield();
void print(char* x, int d1, int d2, int d3);
int strcomp(char* x, char* y);
void kill(int p);
//IPC methods
void sendChan(int pid, int dat);
int getChan(int chan);
int thisId();
#endif
