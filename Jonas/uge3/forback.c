/* 

   Opgave 1

   forback.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "forback.h"

/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[])
{
  int status, pid;
  if ((pid = fork()) == 0)
  {
  	printf("%s\n", argv[0]);
  	printf("sizeof(argv) = %lu\n", sizeof(argv));
  	execvp(filename, argv);
  } else {
  	waitpid(pid, &status, 0);
  }
}

/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[])
{
  int status, pid;
  if ((pid = fork()) == 0)
  {
  	printf("Hej, %s\n", argv[0]);
  	execvp(filename, argv);
  }
}

int main (int argc, char* argv[]) {
    char *args[argc-1];
	int i, pid, status;
	for (i = 1; i < argc; ++i) {
		args[i-1] = argv[i];
	}
	if ((pid = fork()) == 0) {
    printf("Køre foregroundcmd\n");
    foregroundcmd(argv[0], args);
    printf("Foregroundcmd færdig\n");
    } else {
    waitpid(pid, &status, 0);
    printf("-------------------\n");
    printf("Køre backgroundcmd\n");
    backgroundcmd(argv[0], args);
    printf("Backgroundcmd færdig\n");
    }
    exit(0);
}
