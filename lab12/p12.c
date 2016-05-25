#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning (disable : 4996)
#endif

typedef int elementType;


typedef struct LinkList {
	elementType value;
	struct LinkList * next;
}*list;

typedef struct Graph {
	int size;
	list * vertex;
}*graph;

int i, j, k;

list listInit()
{
	list ptr = (list)malloc(sizeof(struct LinkList));
	ptr->next = NULL;
	return ptr;
}
int listSize(list l)
{
	int c = 0;
	list ptr = l;
	for (;l->next != NULL; l = l->next, ++c);
	return c;
}
int listEmpty(list stk)
{
	if (stk->next == NULL)
		return 1;
	return 0;
}
void push(list stk, elementType value)
{
	list ptr = listInit(), tmp;
	ptr->value = value;
	tmp = stk->next;
	stk->next = ptr;
	ptr->next = tmp;
}
void push_back(list stk, elementType value)
{
	list tmp = stk, ptr = listInit();
	ptr->value = value;
	for (;tmp->next != NULL; tmp = tmp->next);
	tmp->next = ptr;
}
elementType top(list stk)
{
	return stk->next->value;
}
elementType back(list stk)
{
	list tmp = stk;
	for (;tmp->next != NULL; tmp = tmp->next);
	return tmp->value;
}
elementType pop(list stk)
{
	if (listEmpty(stk))
		return 0;
	list tmp = stk->next;
	int ret = tmp->value;
	stk->next = tmp->next;
	free(tmp);
	return ret;
}
elementType pop_back(list stk)
{
	list tmp = stk, ptr;
	int ret;
	for (;tmp->next->next != NULL; tmp = tmp->next);
	ptr = tmp->next;
	tmp->next = NULL;
	ret = ptr->value;
	free(ptr);
	return ret;
}
void listFree(list stack)
{
	list ptr = stack->next, tmp = stack;
	for (;ptr != NULL;tmp = ptr, ptr = ptr->next)
		free(tmp);
	free(ptr);
}

graph graphInit(int size)
{
	graph g = (graph)malloc(sizeof(struct Graph));
	g->size = size;
	g->vertex = (list*)malloc(sizeof(list));
	for (i = 0; i < size; ++i)
		g->vertex[i] = listInit();
	return g;
}
void graphAppend(graph g, elementType e, elementType v)
{
	push_back(g->vertex[e], v);
}
void graphTopsortUitl(graph g, int v, int** visited, list* stack)
{
	(*visited)[v] = 1;
	
	list ptr = g->vertex[v]->next;
	for (; ptr != NULL; ptr = ptr->next)
		if (!(*visited)[ptr->value])
			graphTopsortUitl(g, ptr->value, visited, stack);

	push_back(*stack, v);
}
void graphTopsort(graph g)
{
	int * visited = (int*)malloc(sizeof(int) * g->size);
	for (i = 0; i < g->size; ++i)
		visited[i] = 0;
	list stack = listInit();

	for (i = 0; i < g->size; ++i)
		if (!visited[i])
			graphTopsortUitl(g, i, &visited, &stack);

	while (!listEmpty(stack))
		printf("%d\t", pop_back(stack) + 1);
}
void graphFree(graph g)
{
//	free(g);
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

	graph g = NULL;
	list l = listInit();

	int ccc;
	char c = 0, cc = 0;
	for (int ch = 0, chk = 0; !(chk < 0);)
	{
		if (c == '\n')
		{
			switch (cc)
			{
			case 0:
				ccc = listSize(l);
				g = graphInit(ccc);
				break;
			}
			++cc;
		}

		chk = fscanf(fp, "%d", &ch);
		chk = fscanf(fp, "%c", &c);
		if (chk < 0)
			break;

		switch (cc)
		{
		case 0:
			push_back(l, ch);
			break;
		case 1:
			fscanf(fp, "%d", &ccc);
			graphAppend(g, ch-1, ccc-1);

			break;
			/*case 2:
			if (find(t, ch, 0) == NULL)
			printf("no value %d\n", ch);
			else
			printf("%d in table\n", ch);
			*/
		}
	}
	graphTopsort(g);
	printf("\n");


#if defined(_WIN32) && defined(_MSC_VER)
	system("pause");
#endif

	graphFree(g);
	fclose(fp);

	return 0;
}
