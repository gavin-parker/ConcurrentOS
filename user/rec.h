#ifndef __rec_H
#define __rec_H

#include <stddef.h>
#include <stdint.h>

#include "libc.h"
//structure to share data between processes
typedef struct {
  //0 = not in use
  //1 = in use
  //2 = not in use, just changed
  int flag;
  int * data;
} sharedMem;


extern void (*entry_rec)();
#endif
