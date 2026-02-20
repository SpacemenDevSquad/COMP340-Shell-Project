#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int shell_file_exists(char *file_path) {
    struct stat checkedFile;
    return stat(file_path, &checkedFile);;
}

int main (int argc, char *argv[]) {
    char* path = "/usr/bin/aa";
    printf("%d", shell_file_exists(path));
}