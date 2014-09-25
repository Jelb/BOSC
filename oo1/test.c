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


int test1 (void) {
    int pid, in[2], out[2];
    char buffer[128];
    char *argv[1];
    /*input = 1 ; output = 0*/
    if ((pid = fork()) == 0) {
    
        close(in[1]);
        dup2(in[0],0);
        close(out[0]);
        dup2(out[1],1);
        char input[123];
        strcpy(input,"./bosh");
        execvp(input,argv);
    
    } else {
    
        close(in[0]);
        dup2(in[1],1);
        close(out[1]);
        dup2(out[0],0);
        char input[123];
        char quit[20];
        char *cmdline;
        strcpy(input,"ls");
        write(in[1], input, sizeof(input));
        /*strcpy(quit,"quit");
        write(in[1],quit, sizeof(quit));*/
        int n = read(out[0], buffer, sizeof(buffer));
        
        printf("Output: chars: %i : %s\n",n,buffer);
    
    }
}

int main (void) {
    test1();
}
