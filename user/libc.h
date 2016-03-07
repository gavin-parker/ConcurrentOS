#ifndef __LIBC_H
#define __LIBC_H

#include <stddef.h>
#include <stdint.h>
#include "PL011.h"

int write( int fd, void* x, size_t n );
int writeInt( int fd, int x);
int getLen( int x);
int fork();
int exit();
void print(char* x, int d1, int d2, int d3);
#endif
