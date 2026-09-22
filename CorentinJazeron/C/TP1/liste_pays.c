#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("Vous avez listé %d pays, que voici :\n", argc-1);
	for (int i = 1; i < argc; ++i)
	{
		printf("%s\n", argv[i]);
	}
	return 0;
}