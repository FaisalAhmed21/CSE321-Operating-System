#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
  pid_t pid1, pid2, pid3, pid4;
  pid1 = fork();
  if (pid1 == 0){
    printf("Child process ID: %d\n", getpid());
    pid2 = fork();
    
    if (pid2 == 0){
      printf("Grand Child process ID: %d\n", getpid());
    }
    else {
      pid3 = fork();
      if (pid3 == 0){
        printf("Grand Child process ID: %d\n", getpid());
      }
      else {
        pid4 = fork();
        if (pid4 == 0){
          printf("Grand Child process ID: %d\n", getpid());
        }
        
      }  
      
    }
    
  }
  
  else{
    printf("Parent process ID: %d\n", getpid());
  }
  
 }
