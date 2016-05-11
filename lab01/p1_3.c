#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 32

void getName(char *);
int main(int argc, char * argv[])
{
	char (*names)[SIZE];
	int count = 0;

	if (argc < 2)
	{
		printf("you must input argument.\n");
		return -1;
	}

	count = atoi(argv[1]);
	names = (char(*)[SIZE])malloc(sizeof(char[SIZE]) * count);
	memset(names, 0, sizeof(char[SIZE]) * count);

	printf("enter %d names: ", count);

	for (int i = 0; i < count; i++)
		getName(names[i]);

	printf("the names you entered: \n");

	for (int i = 0; i < count; i++)
		printf("%s\n", names[i]);

	return 0;
}
void getName(char * name)
{
	scanf("%s", name);
}