#include "P1.h"

uint32_t gcd( uint32_t x, uint32_t y ) {
  if     ( x == y ) {
    return x;
  }
  else if( x >  y ) {
    return gcd( x - y, y );
  }
  else if( x <  y ) {
    return gcd( x, y - x );
  }
}
void test(){
  print("test \n",0,0,0);
  fork();
  print("done the test \n",0,0,0);
}

void P1() {
  char* str = "P1\n";
  int x = 3;
  print("P1 started\n",0,0,0);
  test();
  print("did a fork and x=%d\n",x,0,0);
  x=6;
  while( 1 ) {    // compute the gcd between pairs of x and y for 2^8 < x, y < 2^24
    /*
    for( uint32_t x = ( 1 << 8 ); x < ( 1 << 24 ); x++ ) {
      for( uint32_t y = ( 1 << 8 ); y < ( 1 << 24 ); y++ ) {
        uint32_t r = gcd( x, y );
        print("gcd( %d, %d) = %d \n",x,y,r);

      }
    }
    */
  }

  return;
}

void (*entry_P1)() = &P1;
