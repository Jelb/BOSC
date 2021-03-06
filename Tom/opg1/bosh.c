/* 

   bosh.c : BOSC shell 

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#include "print.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname(char *hostname)
{


  char *newname = "Tom";

  hostname = newname;

  //while ((*hostname = *newname) != '\0') {
  //  hostname++;
  //  newname++;
  //}
  printf("%c\n", *hostname);
  return hostname;
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  printshellcmd(shellcmd);

  return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX];
  int terminate = 0;
  Shellcmd shellcmd;
  
  //char cmd[100];
  //sprintf(cmd, "/bin/ls");
  //system(cmd);
  


  if (gethostname(hostname)) {

    /* parse commands until exit or ctrl-c */
    while (!terminate) {
      printf("%s", hostname);
      if (cmdline = readline(":# ")) {
	      if(*cmdline) {
	        add_history(cmdline);
	        if (parsecommand(cmdline, &shellcmd)) {
	          terminate = executeshellcmd(&shellcmd);
	        }
	      }
	      free(cmdline);
      } else {
        terminate = 1;
      }
    }

    printf("Exiting bosh.\n");
  }  
  return EXIT_SUCCESS;
}

