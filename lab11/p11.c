#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning (disable : 4996)
#endif

typedef int elementType;
typedef struct Node {
	elementType value;
	struct Node * next;
} * node;

typedef struct Table{
	int size;
	node * lists;
}* table;

int hash(int size, int v)
{
	return v % size;
}
node find(table t, elementType v, int flag)
{
	node n, l;
	
	l = t->lists[hash(t->size, v)];
	n = l->next;
	if (flag && n != NULL)
	{
		printf("collision %d\n", v);
	}

	while (n != NULL && n->value != v)
		n = n->next;

	return n;
}
void insert(table t, elementType v)
{
	node pos, cell, list;
	pos = find(t, v, 1);
	if (pos == NULL)
	{
		cell = (node)malloc(sizeof(struct Node));
		list = t->lists[hash(t->size, v)];
		cell->next = list->next;
		cell->value = v;
		list->next = cell;
	}
}


void destroy(table t)
{

}

int main(int argc, char * argv[])
{
	FILE * fp;

#if !defined(_WIN32) && !defined(_MSC_VER)
	if (argc < 2)
	{
		printf("you must input argument.\n");
		printf("%s [input]\n", argv[0]);
		return -1;
	}
	fp = fopen(argv[1], "r");
#else
	fp = fopen("input.txt", "r");
#endif

	if (fp == NULL)
	{
		printf("%s is not exist.\n", argv[1]);
		return -2;
	}

	table t = NULL;

	char c = 0, cc = 0;
	for (int ch = 0, chk = 0; !(chk < 0);)
	{
		if (c == '\n')
			++cc;

		chk = fscanf(fp, "%d", &ch);
		chk = fscanf(fp, "%c", &c);

		switch (cc)
		{
		case 0:
			t = (table)malloc(sizeof(struct Table));
			t->size = ch;
			t->lists = (node*)malloc(sizeof(struct Node) * t->size);
			for (int i = 0; i < ch; ++i)
			{
				t->lists[i] = (node)malloc(sizeof(struct Node));
				t->lists[i]->next = NULL;
			}
			break;
		case 1:
			insert(t, ch);
			break;
		case 2:
			if (find(t, ch, 0) == NULL)
				printf("no value %d\n", ch);
			else
				printf("%d in table\n", ch);
		}
	}

#if defined(_WIN32) && defined(_MSC_VER)
	system("pause");
#endif

	destroy(t);
	fclose(fp);

	return 0;
}
