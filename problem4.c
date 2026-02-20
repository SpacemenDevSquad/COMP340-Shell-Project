#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>


int shell_execute(char *file_path, char **argv) {
  // execute the file with the command line arguments
  // use the fork() and exec() system call 

  int pid = fork();
  if (pid == 0) {
    execv(file_path, argv);
    exit(-1);
  }
  wait(NULL);
  return 0;
}

int main (int argc, char *argv[]) {
    shell_execute("/bin/echo", argv);
}