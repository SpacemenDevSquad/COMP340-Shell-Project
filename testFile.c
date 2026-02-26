#include <stdio.h>
#include <stdlib.h>


int main (int argc, char *argv[]) {

  if(argc > 1){
    printf("hello %s!\n", argv[1]);
  }else{
    printf("hello unknown\n");
  }
  
  return 0;
}
