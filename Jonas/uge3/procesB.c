#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
    int i;
    for (i=0; i<10; i++) {
        printf("process B\n");
        sleep(2);
    }
    exit(0); 
}
