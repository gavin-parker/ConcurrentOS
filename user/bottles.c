
void bottles(){

for(int i=100;i > 0;i--){
  print("%d bottles of beer on the wall, %d bottles of beer . \nTake one down, pass it around, %d bottles of beer on the wall...\n",i,i,i-1);
}
}

void (*entry_bottles)() = &bottles;
