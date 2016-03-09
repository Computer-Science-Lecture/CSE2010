#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 128

FILE * fin, * fout;

void recursive(char *, size_t, size_t);
void file_put(char *, size_t);
size_t file_get(char *);

void p_swap(char * x, char * y)
{    
	*x = *x ^ *y ^ (*y = *x);
}
int main(int argc, char * argv[])
{
	size_t count; 
	char * string = (char*)malloc(sizeof(char) * SIZE);
	memset(string, 0, sizeof(char) * SIZE);
	if (argc < 2)
	{
		printf("you must input argument.\n");
		printf("%s [input] [output]\n", argv[0]);
		return -1;
	}

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	count = file_get(string);
	recursive(string, 0, count - 1);

	fclose(fin);
	fclose(fout);
	free(string);

	return 0;
}
void recursive(char * string, size_t i, size_t count)
{
	int j;
	if(i == count)
		file_put(string, count+1);
	else
	{
		for(j = i; j <= count; j++)
		{
			p_swap(&string[i], &string[j]);
			recursive(string, i + 1, count);
			p_swap(&string[i], &string[j]);
		}
	}
}
void file_put(char * string, size_t count)
{
	fprintf(fout, "{");
	for(int i = 0; i < count; i++)
		fprintf(fout, "%c%c", string[i], (i + 1 - count) ? ',' : '}');
	fprintf(fout, "\n");
}
size_t file_get(char * string)
{
	size_t ptr = 0, ttr = 0;
	char * ret = (char*)malloc(sizeof(char) * SIZE);
	fscanf(fin, "%s", ret);

	while(ret[++ptr] != '}')
	{
		if(ret[ptr] != ',')
			string[ttr++] = ret[ptr];
	}
		
	for(int i = 0; i < ttr; i++)
		for(int j = i + 1; j < ttr; j++)
			if(string[i] > string[j])
				string[i]^=string[j]^=string[i]^=string[j];

	return ttr;
}