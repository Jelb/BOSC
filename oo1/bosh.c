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
    FILE *hostfile;
    if(hostfile = popen("/bin/hostname","r")) {
        fgets(hostname,HOSTNAMEMAX,hostfile);
        hostname[strlen(hostname)-1] = '\0';
        return hostname;
    }
    
    printf("Hostname could not be located.\n");
    printf("Bosh was terminated.\n");
    return NULL;
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  printshellcmd(shellcmd);

  return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

    char lsres[HOSTNAMEMAX];

    FILE *ls;
    int i = 0;
    if(ls = popen("/bin/ls","r")) {
        while(fgets(lsres,HOSTNAMEMAX,ls)){
        printf("%i: %s",++i,lsres);

        }
    } 
    
    

  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX];
  int terminate = 0;
  Shellcmd shellcmd;
  
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
        } else 
            terminate = 1;
        }
    printf("Exiting bosh.\n");
  }    
    
  return EXIT_SUCCESS;
}

