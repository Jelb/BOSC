#include <stdio.h>
#include <string.h>

#define MAXBUF 512

int match(char [], char []);

main(int argc, char *argv[]) {

	char line[MAXBUF];
	int position;

	FILE *file;
	file = fopen("text.txt","r");

	while(fgets(line, MAXBUF, file)) {
		//position = match(line, *argv[1]);
		//if (position != -1) {
		if (match(line, argv[1]) != 0) {
			printf("%s", line);
		}
	}
	printf("\n");

	fclose(file);
}

int match(char line[], char pattern[]) {
	int c, d, e, line_length, pattern_length, position = -1;
 
    line_length = strlen(line);
    pattern_length = strlen(pattern);
 
    if (pattern_length > line_length) {
        return 0;
    }

    for (c = 0; c <= line_length - pattern_length; c++) {
        position = e = c;
 
        for (d = 0; d < pattern_length; d++) {
            if (pattern[d] == line[e]) {
                e++;
            }
            else {
                break;
            }
        }
        if (d == pattern_length) {
            return 1;
        }
    }
 
    return 0;
}
