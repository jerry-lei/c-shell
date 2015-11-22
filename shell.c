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
  printf("\n-----------");
  printf("\n--C-Shell--");
  printf("\n-----------");
  printf("\n\n");
}

void current_working_directory_print(){
  //getcwd returns newline
  printf("%s >> ", getcwd(current_directory, 1024));
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
    tokens[tok_count] == NULL;
    what_to_do(tokens);

  }
  exit(0);
}

void what_to_do(char * args[]){
  /*
    Looks through args to see what to do.
    Checks if args[n] has any of the special cases(cd, pipe, redirect)
    If it doesn't, uses execvp.
  */
  int c1 = 0;

  if(strcmp(args[0], "exit") == 0)
    exit(0);
  while(args[c1]){
    if((strcmp(args[c1], ">") == 0) || 
       (strcmp(args[c1], "<") == 0) ||
       (strcmp(args[c1], "&") == 0)){
      printf("\nRedirect section\n");
    }
    c1++;
  }
  execvp(args[0], args)   

      


}


