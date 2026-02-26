#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>

#ifndef PATH_MAX
#define PATH_MAX 4096   // Define a reasonable size if not available
#endif


int shell_change_dir(char *dir_path) {
  int change = chdir(dir_path);
  if(change != 0){
    printf("%s\n",strerror(errno));
    return 1;
  }
  return 0;
}


int shell_file_exists(char *file_path) {
    struct stat checkedFile;
    return stat(file_path, &checkedFile);;
}


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
  
  free(path);
  return -1;
}


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
   //run the shell
   
   bool exit = false;
   while (!exit) {
     
    //prints current working directory and waits for user input
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    printf("%s$ ",cwd);
    char bufferArray[250];
    fgets(bufferArray, 249, stdin);

    // Parse the user's input
    char *enteredCommand;
    char *theeUncleansed = bufferArray;
    char *args = strsep(&theeUncleansed, "\n");
    enteredCommand = strsep(&args, " ");
    char *cdArg = args;
    
    //new:
    char *argv[50]; //array of arg strings
    //split into args:
    int arg_ct = 0;
    argv[arg_ct++] = enteredCommand;
    char *token;
    while(args != NULL && arg_ct < 49){
      token = strsep(&args, " ");
      if(token == NULL){  //if there's nothing else in the string
        break;
      }
      if(*token == '\0') continue; //skip empty
      argv[arg_ct++] = token;
    }
    argv[arg_ct] = NULL; //null terminate
    
    

    


    // Actions based on user input

    if(strcmp(enteredCommand,"exit")==0){   // exit
      exit = true;
      
    }else{
    
    if(strcmp(enteredCommand,"cd")==0) {    // cd
      shell_change_dir(cdArg);
      
      
    }else if(shell_file_exists(enteredCommand) == 0) {   // exists in local filesystem
      char cmd[50];
      int res = shell_find_file(enteredCommand, cmd, 50);
      if(res == 0){
        shell_execute(cmd, argv);
      }else if (res == -1){ //could be a path
        shell_execute(enteredCommand, argv);
        
      }
    
    }else{    //exists in path
      
      char cmd[50];
      int res = shell_find_file(enteredCommand, cmd, 50);
      
      if(res == 0){
        shell_execute(cmd, argv);
      }else if (strcmp("", enteredCommand) != 0){
        printf("%s command does not exist\n",enteredCommand);
      }
    }
   }
  }
   
   
   
   /*
	1. display prompt and wait for user input
		// generate some prompt 
		// e.g. username@hostname:~/david$ ping 
	
	2. filter out whitespace command 
	
	3. if command line contains non-whitespace characters
	
	4. parse command line
		if the specified command is “exit”, terminate the program taking care to release any allocated resources.
		if the specified command is “cd”
			change the current working directory to the specified directory path using shell_change_dir().
		if the command is specified using an absolute path (e.g. /usr/bin/ls), exists in the user’s PATH (e.g. ls) or exists in the current folder (e.g. ./hello_world)
			execute the command using shell_execute()
		else
			report an error message
   
   */
    
   
}
