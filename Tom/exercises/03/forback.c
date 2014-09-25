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
	int status;
	pid_t pid = fork();
	if (pid == 0) {
		// child
		execvp(filename, argv);
	} else {
		// parent
		wait(&status);
		exit(0);
	}
}

/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[])
{
	pid_t pid = fork();
	if (pid == 0) {
		// child
		execvp(filename, argv);
	} else {
		// parent
	}
	exit(0);
}

main(int argc, char *argv[])
{
	char *newargv[argc-1];
	int i;
	for (i = 1; i < argc; ++i) {
		newargv[i-1] = argv[i];
	}
	//int j;
	//for (j = 0; j < argc-1; ++j) {
	//	printf("%s\n", newargv[j]);
	//}
	foregroundcmd(argv[1], newargv);
}
