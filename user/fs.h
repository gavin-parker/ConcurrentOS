#include <stddef.h>
#include <stdint.h>
#include "libc.h"

void testFS();

typedef struct{
  char* name;
  uint32_t add;
  int size;
} fd_t;
