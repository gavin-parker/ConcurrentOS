#include <stddef.h>
#include <stdint.h>
#include "libc.h"

void testFS();

typedef struct{
  char name[5];
  uint32_t add;
  int size;
} fd_t;

int readTextFile(char* name, char* buff);
void writeTextFile(char* text, char* name);
void boot();
void list();
void openDirectory(char* name);
void createDirectory(char* name);
