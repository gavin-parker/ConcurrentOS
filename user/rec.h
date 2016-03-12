#ifndef __rec_H
#define __rec_H

#include <stddef.h>
#include <stdint.h>

#include "libc.h"
typedef struct {
  int flag;
  int * data;
} sharedMem;


extern void (*entry_rec)();
#endif
