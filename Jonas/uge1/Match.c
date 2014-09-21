#include <stdio.h>
#include <string.h>

#define MAX 512
int main(int argc, char *args[]) {
    if (argc != 3) {
        printf("Antallet af argumenter er forkert\n- Programmet tager 2 argumenter...");
        return 1;
    }
    
    FILE *file;
    file = fopen(args[2],"r");
    
    char line[MAX];
    char *pattern = args[1];
    
    printf("Search [%s] for pattern [%s]\n",args[2],args[1]);
    while(fgets(line,MAX,file)){
        if(strstr(line, pattern)) {
            printf("%s", line);
        }
    }
    printf("DONE...\n");
}
