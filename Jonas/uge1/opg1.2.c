#include <stdio.h>

int main(int argc, char *args[]){
    if (argc < 2) {
        printf("Fejl: Intet input blev givet!!!\n");
        return 1;
    }
    int i;
    printf("Udskriver argumenter:\n");
    for (i = 0; i < argc; i++) {
        printf("%s\n", args[i]);
    }
}

