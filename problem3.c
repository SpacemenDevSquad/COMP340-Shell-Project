#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <dirent.h>


int shell_find_file(char *file_name, char *file_path, char file_path_size) {
  char *path = strdup(getenv("PATH"));    //list of directories in the PATH
  char *change = path;                    //copy of the pointer to the string that will change
  char *dir_name;
  
  while((dir_name = strsep(&change, ":")) != NULL){    //iterate through the path string, with ":" as a delimiter
    DIR *curr_dir = opendir(dir_name);                 //directory stream for the current path directory
    if(curr_dir == NULL){
      printf("could not open directory from %s\n", dir_name);
      return 1;
    }
    
    struct dirent *dir_file;
    while((dir_file = readdir(curr_dir)) != NULL){     //iterating through the files in the current directory
      if(strcmp(file_name,dir_file->d_name) == 0){      //if the current entry is the file being looked for
      
        int res = snprintf(file_path, (size_t)(file_path_size),"%s/%s",dir_name,file_name);  //adding to the buffer
        //if(res >= file_path_size){
        //  printf("truncated\n");
        //}
        //printf("file: %s\n",file_path);
        
        return 0;
      }
    }
    closedir(curr_dir);
  }
  
  printf("%s does not exist on the PATH\n",file_name);
  free(path);
  return -1;
}






int main(int argc, char *argv[]){

  size_t buffer_size = 50; 
  char buffer[buffer_size];
  int res = shell_find_file(argv[1], buffer, buffer_size);
  if(res == 0){
    printf("path: %s\n",buffer);  //test: firefox, gamemodelist
  }

}







/*
        //check if file path buffer sent in is long enough
        int len = strlen(file_name) + strlen(dir_name) +2;  //1 for the slash and one for the \0
        if(file_path_size <= len){
          //replace ibuffer too short
          char buffer[len];
          file_path = buffer;
        }
*/
