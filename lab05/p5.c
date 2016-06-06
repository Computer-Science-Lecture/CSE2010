#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	int * value;
	int capacity;
	int index;
} Node;

Node * init(int cap)
{
	Node * ptr = (Node*)malloc(sizeof(Node));
	ptr->value = (int*)malloc(sizeof(int) * cap + 1);
	ptr->capacity = cap;
	ptr->index = 0;
	return ptr;
}
void push_back(Node * stk, int value)
{
	if (stk->index + 1 > stk->capacity)
	{
		printf("Insertion Failed: Queue is Full\n");
		return;
	}
	stk->value[stk->index++] = value;
	printf("Insertion Success: %d\n", value);
}
int top(Node * stk)
{
	if(stk->index == 0)
	{
		printf("Print Failed: Queue is Empty\n");
		return 0;
	}
	printf("Top Value: %d\n", stk->value[0]);
	return stk->value[0];
}
int back(Node * stk)
{
	if(stk->index == 0)
	{
		printf("Print Failed: Queue is Empty\n");
		return 0;
	}
	printf("Back Value: %d\n", stk->value[stk->index - 1]);
	return stk->value[stk->index - 1];
}
int pop(Node * stk)
{
	int i, ret = stk->value[0];
	if(stk->index==0)
	{
		printf("Dequeue Failed: Queue is Empty\n");
		return 0;
	}
	for (int i = 1; i < stk->index; i++)
		stk->value[i-1] = stk->value[i];
	stk->index--;
	printf("Dequeue Success: %d\n", ret);
	return ret;
}

void view(Node * stack)
{
	int i;
	printf("view: ");
	for(int i = 0; i< stack->index; i++)
		printf("%d, ", stack->value[i]);
	printf("\n");
}
void empty(Node * stack)
{
	free(stack->value);
	free(stack);
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
	if(fp==NULL)
	{
		printf("%s is not exist.\n", argv[1]);
		return -2;
	}

	char ch;
	int arg, chk = 0;
	Node * queue = (Node*)malloc(sizeof(Node));
	
	do {
		chk = fscanf(fp, "%c", &ch);
		if ( chk < 0)
			break;
		switch (ch)
		{
			case 'e':
				fscanf(fp, "%d", &arg);
				push_back(queue, arg);
				break;
			case 'd':
				pop(queue);
				break;
			case 'f':
				top(queue);
				break;
			case 'r':
				back(queue);
				break;
			case 'n':
				fscanf(fp, "%d", &arg);
				queue = init(arg);
				break;
		}
	} while (ch!=EOF);

	fclose(fp);
	return 0;
}
