#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE * fin, *fout;

typedef struct {
	char name[32];
	double hwavg, exavg;
}student ;

double hw, ex;
void file_put(size_t);
void file_get(size_t);
student * names;

int main(int argc, char * argv[])
{
	size_t count;

	if (argc < 3)
	{
		printf("you must input argument.\n");
		printf("%s [input] [output]\n", argv[0]);
		return -1;
	}

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	fscanf(fin, "%d", &count);
	names = (student*)malloc(sizeof(student) * count);
	file_get(count);
	file_put(count);

	fclose(fin);
	fclose(fout);
	free(names);

	return 0;
}
void file_put(size_t count)
{
	for (int i = 0; i < count; i++)
		fprintf(fout, "%s\nHwAvg %.2lf\nExamAvg %.2lf\n", names[i].name, names[i].hwavg, names[i].exavg);

	fprintf(fout, "\nCourse\nHwAvg %.2lf\nExamAvg %.2lf\n", hw/count, ex/count);
}
void file_get(size_t count)
{
	for (int i = 0; i < count; i++)
	{
		int a, b, c, d, e, f, g, h;
		fscanf(fin, "%s %d %d %d %d %d %d %d %d", names[i].name, &a,&b,&c,&d,&e,&f,&g,&h);
		names[i].hwavg = (a+b+c+d+e+f)/6.;
		names[i].exavg = (g+h)/2.;
		hw += names[i].hwavg;
		ex += names[i].exavg;
	}
}