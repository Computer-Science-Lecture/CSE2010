#include <stdio.h>

void swap_int(int *, int *);
int main(int argc, char * argv[])
{
	int a, b;
	printf("enter 2 integers\n");
	scanf("%d %d", &a, &b);
	printf("you entered\n%d, %d\n", a, b);
	swap_int(&a, &b);
	printf("after swapping\n%d, %d", a, b);
	return 0;
}
void swap_int(int * a, int * b)
{
	*a ^= *b ^= *a ^= *b;
}