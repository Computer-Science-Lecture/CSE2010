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
	return list_node_remove(list, list->first);
}
VALUE list_pop_back(pList list)
{
	return list_node_remove(list, list->last);
}

pNode list_top(pList list)
{
	return list->first;
}
pNode list_back(pList list)
{
	return list->last;
}

typedef int HeapType;
typedef struct Heap {
	int capacity, size;
	HeapType * elements;
}* pHeap;

pHeap heap_create(int cap)
{
	pHeap  heap = (pHeap)malloc(sizeof(struct Heap));
	heap->capacity = cap;
	heap->size = 0;
	heap->elements = (HeapType*)malloc(sizeof(HeapType) * heap->capacity);
	int i;
	for (i = 0; i < heap->capacity; i++)
		heap->elements[i] = INF;
	return heap;
}
void heap_free(pHeap heap)
{
	free(heap->elements);
	free(heap);
}
int heap_empty(pHeap heap)
{
	if (heap->size == 0)
		return 1;
	return 0;
}
int heap_full(pHeap heap)
{
	if (heap->size >= heap->capacity)
		return 1;
	return 0;
}
void heap_push(pHeap heap, HeapType value)
{
	if (heap_full(heap))
	{
		printf("Heap is Full\n");
		return;
	}
	int i;
	for (i = ++(heap->size); heap->elements[i / 2] < value; i /= 2)
		heap->elements[i] = heap->elements[i / 2];
	heap->elements[i] = value;
}
HeapType heap_pop(pHeap heap)
{
	int i, child;
	if (heap->size < 1)
		return 0;
	HeapType max = heap->elements[1], last = heap->elements[heap->size--];

	for (i = 1; i * 2 <= heap->size; i = child)
	{
		child = i * 2;
		if (child != heap->size && heap->elements[child + 1] > heap->elements[child])
			child++;

		if (last < heap->elements[child])
			heap->elements[i] = heap->elements[child];
		else
			break;
	}

	heap->elements[i] = last;
	return max;
}

int list_value(pList list, int index)
{
	int idx = 0;
	LIST_FOR_EACH(list, iter)
	{
		if (idx == index)
			return iter->value;
		++idx;
	}
	return INF;
}
int list_index(pList list, int value)
{
	int idx = 0;
	LIST_FOR_EACH(list, iter)
	{
		if (iter->value == value)
			return idx;
		++idx;
	}
	return 0;
}

void dijkstra(int weight[100][100], pList vertex, int start)	
{
	pList heap = list_create();
	int * dist = (int*)malloc(sizeof(int) * vertex->size), now, now_cost;
	for (i = 0; i < vertex->size; ++i)
		dist[i] = INF;

	list_push(heap, start);
	dist[list_index(vertex, start)] = 0;

	while (heap->size)
	{
		now = heap->first->next->value;
		LIST_FOR_EACH(heap, maxer)
		{
			if (dist[list_index(heap, maxer->value)] < dist[list_index(heap, k)])
				now = maxer->value;
		}
		list_node_remove(heap, list_find(heap, now));

		now_cost = dist[list_index(vertex, now)];

		LIST_FOR_EACH(vertex, iter)
		{
			if (iter->value != now && weight[now][iter->value] != INF)
			{
				int next = iter->value;
				if (dist[list_index(vertex, next)] > now_cost + weight[now][next])
				{
					dist[list_index(vertex, next)] = now_cost + weight[now][next];
					list_push(heap, next);
				}
			}
		}
	}

	printf("start at %d\n", vertex->first->next->value);
	for (i = 0; i < vertex->size; ++i)
		printf("to %d, cost %d\n", list_value(vertex, i), dist[i]);
	printf("\n");

	list_free(heap);
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

	pList vertex = list_create();
	int weight[100][100] = { 0, };
	
	int input_int = 0, temp = 0, line = 0;
	int start = -1;
	char input_char = 0, chk = 1;
	for (;!(chk < 0);)
	{
		//줄바뀜시 실행
		if (input_char == LINE_CHAR)
		{
			switch (line)
			{
				case 0:
					
					LIST_FOR_EACH(vertex, iter)
					{
						LIST_FOR_EACH(vertex, jter)
						{
							weight[iter->value][jter->value] = INF;
						}
					}
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
				list_push_back(vertex, input_int);
				if (start == -1)
					start = input_int;
				break;
			case 1:
				chk = fscanf(fp, "%d", &temp);
				chk = fscanf(fp, "%c", &input_char);
				chk = fscanf(fp, "%d", &k);
				chk = fscanf(fp, "%c", &input_char);
				weight[input_int][temp] = k;
				break;
		}
	}

	dijkstra(weight, vertex, start);

#ifdef _MSC_VER
	system("pause");
#endif
	
	fclose(fp);
	return 0;
}
