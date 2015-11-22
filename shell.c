#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "shells.h"


void start_shell_print(){
  printf("\n|==============================================================================|");
  printf("\n|------------------------------------------------------------------------------|");
  printf("\n|------------------------------------CSHELL------------------------------------|");
  printf("\n|------------------------------------------------------------------------------|");
  printf("\n|==============================================================================|");
  printf("\n\n");
}

void current_working_directory_print(){
  //getcwd returns newline
  printf("[C-SHELL] %s >> ", getcwd(current_directory, 1024));
}

int main(){
  int TOK_LIMIT = 64;
  int MAX_RECEIVED = 1024; 

  char line[MAX_RECEIVED];
  char *tokens[TOK_LIMIT];
  start_shell_print();
  while(1){
    current_working_directory_print();
    fgets(line, MAX_RECEIVED, stdin);
    //If nothing is entered, restart loop.
    if(line[0] == '\n')
      continue;
    //Separate the inputs and put into array, tokens
    char *receive_line = line;
    char *temp_tok;
    int tok_count = 0;
    while(receive_line){
      temp_tok = strsep(&receive_line, " ");
      tokens[tok_count] = strsep(&temp_tok, "\n");
      tok_count++;
    }
    tokens[tok_count] = '\0';
    what_to_do(tokens);

  }
  exit(0);
}

void what_to_do(char * args[]){
  /*
    Looks through args to see what to do.
    Checks if args[n] has any of the special cases(cd, pipe, redirect)
    If it doesn't, runs regular command method.
  */
  int c1 = 0;

  if(strcmp(args[0], "exit") == 0)
    exit(0);
  while(args[c1]){
    //handles redirects
    if((strcmp(args[c1], ">") == 0) || 
       (strcmp(args[c1], "<") == 0) ||
       (strcmp(args[c1], "&") == 0)){
      printf("\nRedirect section\n");
    }
    //handles semicolon separated commands
    if(strcmp(args[c1], ";") == 0)
      printf("\nMultiple Commands\n");
      multiple_commands(args);
    c1++;
  }

  //reg expressions go last(check if special cases first)
  regular_commands(args);   
}

void regular_commands(char *args[]){
  int status, error;
  if(pid = fork())
    error = wait(&status);
  else{
    //printf("\n");
    error = execvp(args[0], args);
    if(error == -1)
      printf("ERROR AT REGULAR_COMMANDS: %s\n", strerror(errno));
  }
}

void multiple_commands(char *args[]){
  char *run_these[10];
  int c1 = 0; //parse through full args
  int c2 = 0; //parse through run_these (resets in while)
  while(args[c1] != '\0'){
    while(strcmp(args[c1], ";") != 0){
      run_these[c2] = args[c1];
      c1++;
      c2++;
    }
    c1++;
    run_these[c2] = '\0';
    regular_commands(run_these);
    while(c2 > 0){
      run_these[c2] = '\0';
      c2--;
    }
  }  
}
    


