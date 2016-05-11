#include <stdio.h>

#define SIZE 32

int main(int argc, char * argv[])
{
	char name[SIZE] = { 0, };
	int ptr = 0;
	printf("enter your name: ");
	do {
		name[ptr] = getchar();
	} while (name[ptr++] != '\n');
	printf("your name is %s", name);
	return 0;
}