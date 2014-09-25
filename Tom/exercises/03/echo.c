#include <stdio.h>

main(int argc, char *argv[])
{
	int i;
	for (i = 1; i < argc-1; ++i)
	{
		printf("%s\n",argv[i]);
	}
}