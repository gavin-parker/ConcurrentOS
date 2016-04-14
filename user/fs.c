#include "fs.h"


void testFS(){

  print("running fs test\n",0,0,0);

  uint32_t blockNum = disk_get_block_num();
  uint32_t blockLen = disk_get_block_len();

  print("block num:%d, block len:%d \n",blockNum,blockLen,0);
  uint8_t a = 7;
  disk_wr( 0,&a, 1 );

  uint8_t b = 0;
  disk_rd( 0,&b, 1 );

  print("a:%d, b:%d\n",a,b,0);
while(1){

}
}
