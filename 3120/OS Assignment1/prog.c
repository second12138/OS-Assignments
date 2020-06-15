/*
Li, Yansong - CSCI3120 - Assignment1
2019.10.07
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

//the maximum length of history list
#define HISTORY 10
//the maximum length of command
#define MAX_LINE 80

//this sturct gives a history command value with the pid number 
typedef struct History{
  char name[MAX_LINE];
  pid_t Pid;
}his;

int fork_(char *arg[], pid_t pid);

int String_Split(char *argv[], char command[]);

//fork a child process
int fork_(char *arg[], pid_t pid){
  //error occurred
  if (pid < 0){
    fprintf(stderr, "Fork Failed");
    return 1;
  //child process
  }else if (pid == 0) {
    if((execvp(arg[0], arg)) == -1){
      printf("%s No such Command!\n", arg[0]);
    }
  }else{
    wait(NULL);
  }
}

//split the input command into parts
int String_Split(char *argv[], char command[]){
  char *delim  = " ";
  int index =0;
  char *token;
  token = strtok(command, delim);
  argv[index] = token;
  while(token){
    token  = strtok(NULL, delim);
    argv[++index] = token;
  }
  return 0;
}

int main(void){
  int i;//int value for loops
  char *args[MAX_LINE/2 +1]; /*command line arguments*/
  //the input commands
  char input[MAX_LINE+1];
  char string[MAX_LINE+1];
  //history array
  his history[HISTORY];
  //flag to determine when to exit program
  int should_run = 1;
  //the totoal number of command
  int total_command = 0;
  int n;
  pid_t pid;
  //keep running the shell
  while(should_run){
    printf("CSCI3120>");
    fflush(stdout);
    //get the input command
    fgets(input, sizeof(input), stdin);
    if (input[strlen(input)-1] == '\n') input[strlen(input)-1] = '\0';
    strcpy(string, input);
    his last_command;
    String_Split(args, input);
    if((strcmp(args[0],"exit")) == 0){//exit the program
      should_run=0;
      return 0;
    }else if((strcmp(args[0],"history")) == 0){//print 10 recently used commands
      if(total_command <= 0){
	printf("There is no command in history\n");
      }else{
	printf("ID\tPID\tCommand\n");
	i = 0;
	while(i < total_command) {
	  printf("%d\t%d\t%s\n", (i+1), history[i].Pid, history[i].name);
	  i++;
        }
      }
    }else if((strcmp(args[0],"!!")) == 0){//run the last command that user have entered
      if(total_command == 0){
        printf("No commands in history\n");
      }else{
	String_Split(args, history[total_command-1].name);
	pid = fork();
	fork_(args,pid);
      }
    }else if(input[0] == '!'){//run the last Nth command that user have entered
      char temp[MAX_LINE];
      strncpy(temp, &input[1],sizeof(input));
      n = atoi(temp);
      if(n > total_command){
	if(total_command == 0){
	  printf("No commands in history\n");
	}else{
	  printf("No such command in history\n");
	}
      }else{
	String_Split(args, history[total_command-n].name);
	pid = fork();
	fork_(args,pid);
      }
    }else{//fork a child process using fork()
      pid = fork();
      //put the command in to history array
      last_command.Pid = pid;
      strcpy(last_command.name, string);
      if(total_command >= HISTORY){
	i=0;
	while(i<HISTORY-1){
	  history[i] = history[i+1];
	  i++;
	}
	history[HISTORY-1] = last_command;
      }else{
	history[total_command] = last_command;
      }
      total_command++;
      if(total_command > HISTORY){
	  total_command = HISTORY;
      }
      fork_(args, pid);
    }
  }
  return 0;
}
