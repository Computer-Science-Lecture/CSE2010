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
	if (queue_isEmpty(queue))
	{
		queue->front = 0;
		queue->rear = -1;
	}
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
// i want include header file,
// but make single file imposed 
///////////////////////////////

typedef int(*Pivot)(int, int);

int pivot_select_left(int first, int last)
{
	if (first == -1 && last == -1)
		printf("%s", __func__);
	else
		return first;
}
int pivot_select_right(int first, int last)
{
	if (first == -1 && last == -1)
		printf("%s", __func__);
	else
		return last;
}
int pivot_select_mid(int first, int last)
{
	if (first == -1 && last == -1)
		printf("%s", __func__);
	else
		return ((first + last) / 2);
}
void quick_sort_print(int ary[], int first, int pivot, int last)
{
	int i = 0;
	printf("<");
	for (i = first; i < pivot - 1; ++i)
		printf("%d, ", ary[i]);
	if (first < pivot)
		printf("%d", ary[pivot - 1]);
	printf("> <%d> <", ary[pivot]);
	for (i = pivot + 1; i < last; ++i)
		printf("%d, ", ary[i]);
	if (pivot < last)
		printf("%d", ary[last]);
	printf(">\n");
	
	//int i = 0;
	//printf("pivot: %d == ", ary[pivot]);
	//for(i = first; i <= last; ++i)
	//	printf("%d, ", ary[i]);
	//printf("\n");
}
int quick_sort_partition(int ary[], int first, int last, Pivot getPivot)
{
	int pivot = getPivot(first, last), i, j;

	for (i = first, j = last;;)
	{
		while (ary[i] <= ary[pivot] && i < last) ++i;
		while (ary[j] > ary[pivot]) --j;
		if (i >= j)
			break;
		if (i != j)
		{
			ary[i] ^= ary[j] ^= (ary[i] ^= ary[j]);
			if (i == pivot)	pivot = j;
			if (j == pivot) pivot = i;
		}

	}
	if(j != pivot)
		ary[pivot] ^= ary[j] ^= (ary[pivot] ^= ary[j]);
	quick_sort_print(ary, first, j, last);
	return j;
}
void quick_sort_util(int ary[], int first, int last, Pivot getPivot)
{
	if (first < last)
	{
		int mid = quick_sort_partition(ary, first, last, getPivot);
		quick_sort_util(ary, first, mid - 1, getPivot);
		quick_sort_util(ary, mid + 1, last, getPivot);
	}
}
void quick_sort(pQueue queue, Pivot getPivot)
{
	printf("quick_sort -method: ");
	getPivot(-1, -1);
	printf("\n");

	int * ary = (int*)malloc(sizeof(int) * queue->size);
	int i = 0;
	QUEUE_WHILE_EMPTY(queue, iter)
	{
		ary[i++] = iter;
	}

	quick_sort_util(ary, 0, i - 1, getPivot);
	int j = 0;
	for (j = 0; j < i; ++j)
		printf("%d, ", ary[j]);
	printf("\n");
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

	pQueue q = queue_create();

	if (fp == NULL)
	{
		printf("%s is not exist.\n", argv[1]);
		return -2;
	}

	int input_int = 0, line = -1;
	char input_char = LINE_CHAR, chk = 1, input_string[100], inputed_string = 0;
	for (;!(chk < 0);)
	{
		//줄바뀜시 실행
		if (input_char == LINE_CHAR)
		{
			fscanf(fp, "%s", input_string);
			switch (inputed_string)
			{
				case 'l':
					quick_sort(q, pivot_select_left);
					break;
				case 'r':
					quick_sort(q, pivot_select_right);
					break;
				case 'm':
					quick_sort(q, pivot_select_mid);
					break;
			}
			inputed_string = input_string[0];
			++line;
		}

		chk = fscanf(fp, "%d", &input_int);
		chk = fscanf(fp, "%c", &input_char);

		//매 입력마다 실행
		queue_push(q, input_int);

		if (chk < 0)
			break;
	}

	switch (inputed_string)
	{
		case 'l':
			quick_sort(q, pivot_select_left);
			break;
		case 'r':
			quick_sort(q, pivot_select_right);
			break;
		case 'm':
			quick_sort(q, pivot_select_mid);
			break;
	}


#ifdef _MSC_VER
	system("pause");
#endif

	fclose(fp);
	return 0;
}
