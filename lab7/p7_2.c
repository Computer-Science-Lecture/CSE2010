#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

#define INF 987654321
typedef int HeapType;
typedef struct Heap {
	int capacity, size;
	HeapType * elements;
} Heap;

Heap * init(int cap)
{
	Heap *  heap = (Heap*)malloc(sizeof(Heap));
	heap->capacity = cap;
	heap->size = 0;
	heap->elements = (HeapType*)malloc(sizeof(HeapType) * heap->capacity);
	int i;
	for (i = 0; i < heap->capacity; i++)
		heap->elements[i] = INF;
	return heap;
}
int isFull(Heap * heap)
{
	if (heap->size >= heap->capacity)
		return 1;
	return 0;
}
void insert(Heap * heap, HeapType value)
{
	if (isFull(heap))
	{
		printf("Heap is Full\n");
		return;
	}
	int i;
	for (i = ++(heap->size); heap->elements[i / 2] < value; i /= 2)
		heap->elements[i] = heap->elements[i / 2];
	heap->elements[i] = value;
}
HeapType deleteValue(Heap * heap)
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
void printHeap(Heap * heap, int node)
{
	if (node >= heap->capacity || heap->elements[node] == INF)
		return;
	printf("%d ", heap->elements[node]);
	printHeap(heap, node * 2);
	printHeap(heap, node * 2 + 1);
}
int main(int argc, char * argv[])
{
	FILE * fp;
	if (argc < 2)
	{
		printf("you must input argument.\n");
		printf("%s [input]\n", argv[0]);
		return -1;
	}
	fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		printf("%s is not exist.\n", argv[1]);
		return -2;
	}

	Heap * heap = NULL;

	char ch;
	int chk, arg;
	do {
		chk = fscanf(fp, "%c", &ch);
		if (chk < 0)
			break;
		switch (ch)
		{
		case 'n':
			fscanf(fp, "%d", &arg);
			heap = init(arg);
			break;
		case 'i':
			fscanf(fp, "%d", &arg);
			insert(heap, arg);
			break;
		case 'd':
			fscanf(fp, "%d", &arg);
			deleteValue(heap);
			break;
		case 'p':
			fscanf(fp, "%c", &ch);
			printHeap(heap, 1);
			printf("\n");
			break;
		}
	} while (ch != EOF);

	fclose(fp);
	free(heap->elements);
	free(heap);
	return 0;
}
