#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


#include <errno.h>



int shell_change_dir(char *dir_path) {
  // use the chdir() system call to change the current directory
  int change = chdir(dir_path);
  if(change != 0){
    printf("%s\n",strerror(errno));
    return 1;
  }
  return 0;
}


int main (int argc, char *argv[]) {
    shell_change_dir(".");
}
