/* 
   bosh.c : BOSC shell
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/*Handles when user presses CTRL + C*/
void child_int (int signal) {
  exit(0);
}

void parent_int (int signal) {
  printf("\nChild processes has exited.\n");
}


char *gethostnamecmd(char *hostname)
{ 
  FILE *hostfile = popen("/bin/hostname","r");
  fgets(hostname,HOSTNAMEMAX,hostfile);
  hostname[strlen(hostname)-1] = '\0';
  pclose(hostfile);
  return hostname;
}

char *getusernamecmd(char *username)
{ 
  FILE *userfile  = popen("/usr/bin/id -un","r");
  fgets(username,HOSTNAMEMAX,userfile);
  username[strlen(username)-1] = '\0';
  pclose(userfile);
  return username;
}

void executeshellcmd (Shellcmd *shellcmd) {

  int status, fd[2];
  int pid = 1;
  Cmd *cmd = shellcmd->the_cmds;
  shellcmd->the_cmds = shellcmd->the_cmds->next;

  if (cmd->next != NULL) {
    pipe(fd);
    pid++;
     
    if ((pid = fork()) == 0) {
      close(fd[0]);
      dup2(fd[1],1);
      executeshellcmd(shellcmd);
    } else {
      close(fd[1]);
      dup2(fd[0],0);
      waitpid(pid, &status, 0);
    }
  }
  
  execvp(cmd->cmd[0], cmd->cmd);

  /*This will only run if execvp fails*/
  printf("Command \"%s\" not found\n",*cmd->cmd);
  /*Prevent child from start listening for commands*/
  exit(0);
}

/* --- execute a shell command --- */
int initializeExecution (Shellcmd *shellcmd)
{
  int status, pid;
  Cmd *cmd = shellcmd->the_cmds;
  
  /*Terminate bosh if the cmd matches exit or quit*/
  if (strcmp(*cmd->cmd,"exit") == 0) return 1;
  if (strcmp(*cmd->cmd,"quit") == 0) return 1;
  
  /*Abort if cmd is to start new instance of bosh*/
  if (strncmp(*cmd->cmd,"./bosh",6)== 0) {
    printf("Cannot start new instance of bosh inside this.\n");
    printf("Command aborted...\n");
    return 0;
  }
  
  /*Creates child to execute shell command*/
  if (!(pid = fork()) == 0) {
    /*Waits if child is not a background process*/
    if (!(shellcmd->background)) waitpid(pid, &status, 0);
  } else {
    /*Set interrupt handler for child processes*/
    signal(SIGINT, child_int);
      
    if (shellcmd->rd_stdin != NULL)
      freopen(shellcmd->rd_stdin, "r", stdin);
    
    if (shellcmd->rd_stdout != NULL) {
      /*Redirect stdout to out specified by shellcmd if on one command*/
      int outdir = open(shellcmd->rd_stdout, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
      dup2(outdir,1);
    }
    executeshellcmd(shellcmd);
  }
  return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {
  /*Listens for CTRL + C signal*/
  signal(SIGINT, parent_int);
  
  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX];
  char username[HOSTNAMEMAX];
  int terminate = 0;
  Shellcmd shellcmd;
  
  if (gethostnamecmd(hostname) && getusernamecmd(username)) {
    /* parse commands until exit or ctrl-c */
    while (!terminate) {
      printf("%s@%s", username, hostname);
      if (cmdline = readline(":# ")) {
	      if(*cmdline) {
	        add_history(cmdline);
	        if (parsecommand(cmdline, &shellcmd))
	          terminate = initializeExecution(&shellcmd);
	      }
	      free(cmdline);
      } else 
        terminate = 1;
      }
    printf("Exiting bosh.\n");
  }
  return EXIT_SUCCESS;
}