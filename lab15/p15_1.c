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

///////////////////////////////
//headers
#ifndef QUEUE_VALUE
#define QUEUE_VALUE int
#endif

#ifndef QUEUE_CAPACITY
#define QUEUE_CAPACITY 16
#endif

typedef QUEUE_VALUE __QUEUE__VALUE__;

typedef struct Queue {
	int capacity, size, front, rear;
	__QUEUE__VALUE__ * raw;
}*pQueue;

#define QUEUE_WHILE_EMPTY(queue, iter)\
	__QUEUE__VALUE__ (iter);\
	while(!queue_isEmpty((queue)) && (((iter) = queue_pop((queue))) || 1))

pQueue queue_create();
void queue_free(pQueue);

void queue_realloc(pQueue, int);
int queue_isFull(pQueue);
int queue_isEmpty(pQueue);

void queue_push(pQueue, __QUEUE__VALUE__);
__QUEUE__VALUE__ queue_pop(pQueue);

__QUEUE__VALUE__ queue_top(pQueue);
__QUEUE__VALUE__ queue_back(pQueue);
pQueue queue_create()
{
	pQueue queue = (pQueue)malloc(sizeof(struct Queue));
	queue->capacity = QUEUE_CAPACITY;
	queue->size = 0;
	queue->front = 0;
	queue->rear = -1;
	queue->raw = (__QUEUE__VALUE__*)malloc(sizeof(__QUEUE__VALUE__) * queue->capacity);

	return queue;
}
void queue_free(pQueue queue)
{
	free(queue->raw);
	free(queue);
}
void queue_realloc(pQueue queue, int size)
{
	if (queue->size >= size)
		return;
	__QUEUE__VALUE__ * temp = (__QUEUE__VALUE__*)malloc(sizeof(__QUEUE__VALUE__) * size);
	int i;
	for (i = 0; i < queue->size; ++i)
		temp[i] = queue->raw[(i + queue->front) % queue->capacity];

	queue->capacity = size;
	free(queue->raw);
	queue->raw = temp;
	queue->front = 0;
	queue->rear = queue->size - 1;
}
int queue_isFull(pQueue queue)
{
	if (queue->size == queue->capacity)
		return 1;
	return 0;
}
int queue_isEmpty(pQueue queue)
{
	if (queue->size == 0)
		return 1;
	return 0;
}

void queue_push(pQueue queue, __QUEUE__VALUE__ value)
{
	if (queue_isFull(queue))
		queue_realloc(queue, queue->capacity + QUEUE_CAPACITY);
	queue->raw[++queue->rear] = value;
	queue->rear %= queue->capacity;
	++queue->size;
}
__QUEUE__VALUE__ queue_pop(pQueue queue)
{
	if (queue->size < queue->capacity - QUEUE_CAPACITY * 2)
		queue_realloc(queue, queue->capacity - QUEUE_CAPACITY);
	int ret = queue->raw[queue->front++];
	queue->front %= queue->capacity;
	--queue->size;
	return ret;
}

__QUEUE__VALUE__ queue_top(pQueue queue)
{
	return queue->raw[queue->front];
}
__QUEUE__VALUE__ queue_back(pQueue queue)
{
	return queue->raw[queue->rear];
}
//headers
///////////////////////////////

void merge_sort_merge(int ary[], int first, int mid, int last)
{
	int i, j, k;
	int n1 = mid - first + 1;
	int n2 = last - mid;

	int * left = (int*)malloc(sizeof(int) * n1),
		*right = (int*)malloc(sizeof(int) * n2);

	for (i = 0; i < n1; ++i)
		left[i] = ary[first + i];
	for (i = 0; i < n2; ++i)
		right[i] = ary[mid + 1 + i];

	i = 0; j = 0; k = first;
	while (i < n1 && j < n2)
	{
		if (left[i] <= right[j])
			ary[k++] = left[i++];
		else
			ary[k++] = right[j++];
	}

	while (i < n1)
		ary[k++] = left[i++];

	while (j < n2)
		ary[k++] = right[j++];

	for (i = first; i <= last; ++i)
		printf("%d, ", ary[i]);
	printf("\n");
}
void merge_sort_iterative_util(int ary[], int n)
{
	int current, start;

	for (current = 1; current <= n; current *= 2)
		for (start = 0; start < n; start += 2 * current)
		{
			int mid = start + current - 1;
			int right = (start + 2 * current > n) ? (n) : (start + 2 * current);

			merge_sort_merge(ary, start, mid, right);
		}

}
void merge_sort_iterative(pQueue queue)
{
	printf("merge_sort_iterative\n");
	int * ary = (int*)malloc(sizeof(int) * queue->size);
	int i = 0;
	QUEUE_WHILE_EMPTY(queue, iter)
	{
		ary[i++] = iter;
	}
	merge_sort_iterative_util(ary, i - 1);
}
void merge_sort_recursive_util(int ary[], int first, int last)
{
	if (first < last)
	{
		int m = first + (last - first) / 2;
		merge_sort_recursive_util(ary, first, m);
		merge_sort_recursive_util(ary, m + 1, last);
		merge_sort_merge(ary, first, m, last);
	}
}
void merge_sort_recursive(pQueue queue)
{
	printf("merge_sort_recursive\n");
	int * ary = (int*)malloc(sizeof(int) * queue->size);
	int i = 0;
	QUEUE_WHILE_EMPTY(queue, iter)
	{
		ary[i++] = iter;
	}
	merge_sort_recursive_util(ary, 0, i - 1);
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

	pQueue p = queue_create(), q = queue_create();

	if (fp == NULL)
	{
		printf("%s is not exist.\n", argv[1]);
		return -2;
	}

	int input_int = 0, line = 0;
	char input_char = 0, chk = 1;
	for (;!(chk < 0);)
	{
		//줄바뀜시 실행
		if (input_char == LINE_CHAR)
			++line;

		chk = fscanf(fp, "%d", &input_int);
		chk = fscanf(fp, "%c", &input_char);


		//매 입력마다 실행
		switch (line)
		{
		case 0:
			queue_push(q, input_int);
			queue_push(p, input_int);
			break;
		}

		if (chk < 0)
			break;
	}

	merge_sort_iterative(p);
	merge_sort_recursive(q);

#ifdef _MSC_VER
	system("pause");
#endif

	fclose(fp);
	return 0;
}
