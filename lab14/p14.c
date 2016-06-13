#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) && defined(_MSC_VER)
	#pragma warning (disable : 4996)
#endif

#if defined(__linux__)
	#define LINE_CHAR 10
#elif defined(_MSC_VER)
	#define LINE_CHAR '\n'
#else
	#define LINE_CHAR 13
#endif

#ifndef LIST_VALUE
#define LIST_VALUE int
#endif

#ifndef LIST_EQUAL
#define LIST_EQUAL(x, y) ((x) == (y))
#endif

#ifndef LIST_DEFAULT
#define LIST_DEFAULT 0
#endif

typedef LIST_VALUE VALUE;

typedef struct Node {
	VALUE value;
	struct Node * next, *front;
}*pNode;

typedef struct List {
	int size;
	pNode first, last;
}*pList;

// LIST_FOR_EACH(list, iterator)
// throw node in list, sequentially
#define LIST_FOR_EACH(LIST, iter)	for (pNode (iter) = (LIST)->first->next; (iter) != (LIST)->last; (iter) = (iter)->next)

// LIST_FOR_EACH_REVERSE(list, iterator)
// throw node in list, reverse order
#define LIST_FOR_EACH_REVERSE(LIST, iter)	for (pNode (iter) = (LIST)->last->front; (iter) != (LIST)->first; (iter) = (iter)->front)

int i, j, k;
#define INF 99999

pNode list_node_create(VALUE value)
{
	pNode node = (pNode)malloc(sizeof(struct Node));
	node->next = node->front = NULL;
	node->value = value;
	return node;
}
void list_node_free(pNode node)
{
	if (node == NULL)
		return;
	free(node);
}

pList list_create()
{
	pList list = (pList)malloc(sizeof(struct List));
	list->first = list_node_create(LIST_DEFAULT);
	list->last = list_node_create(LIST_DEFAULT);
	list->first->next = list->last;
	list->last->front = list->first;
	list->size = 0;
	return list;
}
VALUE list_node_remove(pList list, pNode node)
{
	if (node == NULL)
		return LIST_DEFAULT;

	pNode front = node->front, next = node->next;
	VALUE ret = node->value;

	if (front != NULL)
		front->next = next;
	if (next != NULL)
		next->front = front;

	list_node_free(node);
	--list->size;
	return ret;
}
void list_free(pList list)
{
	pNode temp = NULL;
	LIST_FOR_EACH(list, iter)
	{
		list_node_remove(list, temp);
		temp = iter;
	}
	list_node_remove(list, temp);
	free(list->first);
	free(list->last);
	free(list);
}

pNode list_find(pList list, LIST_VALUE value)
{
	LIST_FOR_EACH(list, iter)
	{
		if (LIST_EQUAL(iter->value, value))
			return iter;
	}
	return NULL;
}

void list_node_insert(pList list, pNode tar, pNode node)
{
	pNode front = tar->front;
	tar->front = node;
	if (front != NULL)
		front->next = node;
	node->front = front;
	node->next = tar;
	++list->size;
}
void list_node_insert_back(pList list, pNode tar, pNode node)
{
	pNode next = tar->next;
	tar->next = node;
	if (next != NULL)
		next->front = node;
	node->front = tar;
	node->next = next;
	++list->size;
}

void list_push(pList list, VALUE value)
{
	list_node_insert_back(list, list->first, list_node_create(value));
}
void list_push_back(pList list, VALUE value)
{
	list_node_insert(list, list->last, list_node_create(value));
}
void list_push_node(pList list, pNode node)
{
	list_node_insert_back(list, list->first, node);
}
void list_push_node_back(pList list, pNode node)
{
	list_node_insert(list, list->last, node);
}

VALUE list_pop(pList list)
{
	return list_node_remove(list, list->first->next);
}
VALUE list_pop_back(pList list)
{
	return list_node_remove(list, list->last->front);
}

pNode list_top(pList list)
{
	return list->first->next;
}
pNode list_back(pList list)
{
	return list->last->front;
}
int list_empty(pList list)
{
	if (list->first->next == list->last)
		return 1;
	return 0;
}

typedef struct Graph {
	int size;
	pList * vertex;
}*pGraph;

pGraph graph_create()
{
	pGraph graph = (pGraph)malloc(sizeof(struct Graph));
	graph->size = 0;
	graph->vertex = NULL;
}
void graph_realloc(pGraph graph, int size)
{
	pList * temp = (pList*)malloc(sizeof(pList) * size);
	for (i = 0; i < graph->size; ++i)
		temp[i] = graph->vertex[i];
	free(graph->vertex);
	graph->vertex = temp;
	graph->size = size;
}
void graph_insert_vertex(pGraph graph, int vertex)
{
	graph_realloc(graph, graph->size + 1);
	graph->vertex[graph->size - 1] = list_create();
	graph->vertex[graph->size - 1]->first->value = vertex;
}
void graph_append_edge(pGraph graph, int start, int to)
{
	if (start > graph->size)
		return;
	for (i = 0; i < graph->size; ++i)
		if (graph->vertex[i]->first->value == start)
		{
			list_push_back(graph->vertex[i], to);
			break;
		}
}
void graph_find_dfs_rec_util(pGraph graph, int now, int to, int * visited)
{
	int now_index;
	for (now_index = 0; graph->vertex[now_index]->first->value != now && now_index < graph->size; ++now_index);
	if (visited[now] != 0)
		return;
	visited[now] = 1;
	printf("visit: %d\n", now);
	if (now == to)
		return;
	LIST_FOR_EACH(graph->vertex[now_index], iter)
	{
		graph_find_dfs_rec_util(graph, iter->value, to, visited);
	}
}
void graph_find_dfs_rec(pGraph graph, int start, int to)
{
	printf("find path %d to %d using dfs - recursive function\n", start, to);
	int * visited = (int*)malloc(sizeof(int) * graph->size);
	for(i = 0; i < graph->size; ++i)
		visited[i] = 0;

	graph_find_dfs_rec_util(graph, start, to, visited);
	free(visited);
}
void graph_find_dfs(pGraph graph, int start, int to)
{
	printf("find path %d to %d using dfs\n", start, to);
	int now, now_index;
	int * visited = (int*)malloc(sizeof(int) * graph->size);
	for (i = 0; i < graph->size; ++i)
		visited[i] = 0;
	pList stack = list_create();
	list_push(stack, start);
	while (!list_empty(stack))
	{
		int now = list_pop(stack);
		for (now_index = 0; graph->vertex[now_index]->first->value != now && now_index < graph->size; ++now_index);
		if (visited[now] != 0)
			continue;
		visited[now] = 1;
		printf("visit: %d\n", now);
		if (now == to)
			break;
		LIST_FOR_EACH(graph->vertex[now_index], iter)
		{
			list_push(stack, iter->value);
		}
	}
	list_free(stack);
	free(visited);
}
void graph_find_bfs(pGraph graph, int start, int to)
{
	printf("find path %d to %d using bfs\n", start, to);
	int now, now_index;
	int * visited = (int*)malloc(sizeof(int) * graph->size);
	for (i = 0; i < graph->size; ++i)
		visited[i] = 0;
	pList queue = list_create();
	list_push_back(queue, start);
	while (!list_empty(queue))
	{
		int now = list_pop(queue);
		for (now_index = 0; graph->vertex[now_index]->first->value != now && now_index < graph->size; ++now_index);
		if (visited[now] != 0)
			continue;
		visited[now] = 1;
		printf("visit: %d\n", now);
		if (now == to)
			break;
		LIST_FOR_EACH(graph->vertex[now_index], iter)
		{
			list_push_back(queue, iter->value);
		}
	}
	list_free(queue);
	free(visited);
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

	pGraph graph = graph_create();

	int input_int = 0, line = 0;
	char input_char = 0, chk = 1;
	for (;!(chk < 0);)
	{
		//줄바뀜시 실행
		if (input_char == LINE_CHAR)
		{
			switch (line)
			{
			case 0:
				break;
			}
			++line;
		}

		chk = fscanf(fp, "%d", &input_int);
		chk = fscanf(fp, "%c", &input_char);

		if (chk < 0)
			break;

		//매 입력마다 실행
		switch (line)
		{
			case 0:
				graph_insert_vertex(graph, input_int);
				break;
			case 1:
				chk = fscanf(fp, "%d", &k);
				chk = fscanf(fp, "%c", &input_char);
				graph_append_edge(graph, input_int, k);
				break;
			case 2:
				chk = fscanf(fp, "%d", &k);
				chk = fscanf(fp, "%c", &input_char);
				graph_find_bfs(graph, input_int, k);
				graph_find_dfs(graph, input_int, k);
				graph_find_dfs_rec(graph, input_int, k);
				break;
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif

	fclose(fp);
	return 0;
}
