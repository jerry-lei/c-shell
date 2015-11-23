#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "utils.h"


void start_shell_print(){
  printf("\n|==============================================================================|");
  printf("\n|------------------------------------------------------------------------------|");
  printf("\n|------------------------------------CSHELL------------------------------------|");
  printf("\n|------------------------------------------------------------------------------|");
  printf("\n|==============================================================================|");
  printf("\n\n");
}

void current_working_directory_print(){
  /*
    Looped in main
    Gets the current working directory
  */
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
    //makes everything from tokens[tok_count] onwards to '\0'
    while(tok_count < TOK_LIMIT){
      tokens[tok_count] = '\0';
      tok_count++;
    }
    if(strcmp(tokens[0], "cd") == 0){
      change_directory(tokens);
      continue;
    }
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
    
    if(strcmp(args[c1], ";") == 0){
      multiple_commands(args);
      return;
    }

    c1++;
    
  }

  //reg expressions go last(check if special cases first)
  regular_commands(args);   
}

void regular_commands(char *args[]){
  /*
    Used in multiple_commands and what_to_do if none of the special cases apply
    Takes in args with terminating null 
    forks and execvp
  */
  int status, error;
  if(pid = fork())
    error = wait(&status);
  else{
    error = execvp(args[0], args);
    if(error == -1)
      printf("ERROR AT REGULAR_COMMANDS: %s\n", strerror(errno));
  }
}

void multiple_commands(char *args[]){
  /*
    Used in what_to_do if args contains a ";"
    Runs regular_commands by separating into separate arrays of arguments
  */
  char *run_these[10];
  int c1 = 0; //count through full args
  int c2 = 0; //count through run_these (resets in while)
  while(args[c1]){
    while(args[c1] != '\0' && strcmp(args[c1], ";")){
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
    
void change_directory(char *args[]){
  /*
    In main loop and used when arg[0] is "cd"
    Changes directory
  */
  if(args[1] == '\0'){
    chdir(getenv("HOME"));
    return;
  }
  //if it args[1] is doable, chdir
  //if args[1] is not doable, returns 'is not a directory'
  else
    if(chdir(args[1]) == -1){
      printf("%s is not a directory\n", args[1]);
      return;
    }
}
