/* 

   bosh.c : BOSC shell 

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"


/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname(char *hostname)
{ 
    FILE *hostfile = popen("/bin/hostname","r");
    fgets(hostname,HOSTNAMEMAX,hostfile);
    hostname[strlen(hostname)-1] = '\0';
    pclose(hostfile);
    return hostname;
}

char *getusername(char *username)
{ 
    FILE *userfile  = popen("/usr/bin/id -un","r");
    fgets(username,HOSTNAMEMAX,userfile);
    username[strlen(username)-1] = '\0';
    pclose(userfile);
    return username;
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  int status, pid;
  
  char *argv[0];
  Cmd *cmd = shellcmd->the_cmds;
  printf("Command: %s\n",*cmd->cmd);
  
  /*Terminate bosh if the cmd matches exit or quit*/
  if (strcmp(*cmd->cmd,"exit") == 0) return 1;
  if (strcmp(*cmd->cmd,"quit") == 0) return 1;
  
  if (!(pid = fork()) == 0) {
    if (!(shellcmd->background)) waitpid(pid, &status, 0);
  } else {
  
    if (cmd->next != NULL) {
        printf("multiple functions.\n");
    } 
  
    execvp(cmd->cmd[0], cmd->cmd);
    printf("Command not found\n");
    exit(0);
    
  }
  
  printshellcmd(shellcmd);
  
  return 0;
}
/*Handles when user presses CTRL + C*/
void interrupt (int signal) {
    printf("Du har trygget CTRL + C: %i", signal);
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

    /*Listens for CTRL + C signal*/
    signal(SIGINT, interrupt);
    

  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX];
  char username[HOSTNAMEMAX];
  int terminate = 0;
  Shellcmd shellcmd;
  
  if (gethostname(hostname) && getusername(username)) {

    /* parse commands until exit or ctrl-c */
    while (!terminate) {
        printf("%s@%s", username, hostname);
        if (cmdline = readline(":# ")) {
	        if(*cmdline) {
	            add_history(cmdline);
	            if (parsecommand(cmdline, &shellcmd)) {
	                terminate = executeshellcmd(&shellcmd);
	            }
	        }
	        free(cmdline);
        } else 
            terminate = 1;
        }
    printf("Exiting bosh.\n");
  }    
    
  return EXIT_SUCCESS;
}

