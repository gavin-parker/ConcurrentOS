#ifndef __LIBC_H
#define __LIBC_H

#include <stddef.h>
#include <stdint.h>
#include "PL011.h"

int read(int fd, void *x, size_t n);
int write( int fd, void* x, size_t n );
int writeInt( int fd, int x);
int getLen( int x);
int fork();
int exit();
void yield();
void print(char* x, int d1, int d2, int d3);
int strcomp(char* x, char* y);
void kill(int p);
void share(int pid, int add);
#endif
