#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 32

FILE * fin, *fout;

typedef struct students {
	char name[SIZE];
	int literature, math, science;
}student ;

void file_put(size_t);
size_t file_get();
student * names;
int main(int argc, char * argv[])
{
	size_t count;

	if (argc < 2)
	{
		printf("you must input argument.\n");
		printf("%s [input] [output]\n", argv[0]);
		return -1;
	}

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	count = file_get();
	
	file_put(count);

	fclose(fin);
	fclose(fout);
	free(names);

	return 0;
}
void file_put(size_t count)
{
	fprintf(fout, "Name\t");
	for (int i = 0; i < count; i++)
		fprintf(fout, "%s\t", names[i].name);
	fprintf(fout, "\nLiterature\t");
	for (int i = 0; i < count; i++)
		fprintf(fout, "%d\t", names[i].literature);
	fprintf(fout, "\nMath\t");
	for (int i = 0; i < count; i++)
		fprintf(fout, "%d\t", names[i].math);
	fprintf(fout, "\nScience\t");
	for (int i = 0; i < count; i++)
		fprintf(fout, "%d\t", names[i].science);
	fprintf(fout, "\n");
}
size_t file_get()
{
	size_t count;
	fscanf(fin, "%d", &count);
	names = (student*)malloc(sizeof(student) * count);
	for (int i = 0; i < count; i++)
		fscanf(fin, "%s %d %d %d", names[i].name, &names[i].literature, &names[i].math, &names[i].science);

	return count;
}