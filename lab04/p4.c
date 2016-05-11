#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	int value;
	struct Node * next;
} Node;

int getOper(char value)
{
	switch (value)
	{
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
		case '%':
			return 2;
		case '(':
			return 0;
		case ')':
			return 3;
	}
}
Node * init()
{
	Node * ptr = (Node*)malloc(sizeof(Node));
	ptr->next = NULL;
	return ptr;
}
int isEmpty(Node * stk)
{
	if (stk->next == NULL)
		return 1;
	return 0;
}
void push(Node * stk, int value)
{
	Node * ptr = init(), * tmp;
	ptr->value = value;
	tmp = stk->next;
	stk->next = ptr;
	ptr->next = tmp;
}
void push_back(Node * stk, int value)
{
	Node *tmp = stk, *ptr = init();
	ptr->value = value;
	for (;tmp->next != NULL; tmp = tmp->next);
	tmp->next = ptr;
}
int top(Node * stk)
{
	return stk->next->value;
}
int back(Node * stk)
{
	Node *tmp = stk;
	for (;tmp->next != NULL; tmp = tmp->next);
	return tmp->value;
}
int pop(Node * stk)
{
	if (isEmpty(stk))
		return 0;
	Node * tmp = stk->next;
	int ret = tmp->value;
	stk->next = tmp->next;
	free(tmp);
	return ret;
}
int pop_back(Node * stk)
{
	Node * tmp = stk, *ptr;
	int ret;
	for (;tmp->next->next != NULL; tmp = tmp->next);
	ptr = tmp->next;
	tmp->next = NULL;
	ret = ptr->value;
	free(ptr);
	return ret;
}
int oper(char ch, int first, int second)
{
	switch (ch)
	{
		case '+':
			return first+second;
		case '-':
			return first-second;
		case '*':
			return first*second;
		case '/':
			return first/second;
		case '%':
			return first%second;
	}
	return 0;
}
int calc(Node * stack)
{
	Node * number = init();
	int first, second;
	while(!isEmpty(stack))
	{
		int e = pop(stack);
		switch (e)
		{
			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
				push(number, oper(e, pop(number), pop(number)));
				break;
			default:
				push(number, e);

		}
	}
	return pop(number);
}
void view(Node * stack)
{
	Node * ptr = stack->next;
	printf("\nconverted postfix form: ");
	for (;ptr != NULL; ptr = ptr->next)

	{
		if (ptr->value > 10)
			printf("%c", ptr->value);
		else
			printf("%d", ptr->value);

	}
	printf("#\n");
}
void empty(Node * stack)
{
	Node * ptr = stack->next, *tmp = stack;
	for (;ptr!=NULL;tmp = ptr, ptr=ptr->next)
		free(tmp);
	free(ptr);
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

	char ch, op;
	Node * stack = init(), *posfix = init();
	printf("original infix form: ");
	while (1)
	{
		fscanf(fp, "%c", &ch);
		if (ch > '0' && ch <= '9')
			push_back(posfix, ch - '0');
		else
		{
			switch (ch)
			{
				case '(':
					push(stack, '(');
					break;
				case ')':
					while (!isEmpty(stack) && top(stack) != '(')
						push_back(posfix, pop(stack));
					pop(stack);
					break;
				case '*':
				case '/':
				case '+':
				case '-':
				case '%':
					while (!isEmpty(stack) && getOper(ch) <= getOper(top(stack)))
						push_back(posfix, pop(stack));
					push(stack, ch);
				break;
				case '\n':
				case ' ':
				case EOF:
					printf("#");
					while (!isEmpty(stack))
						push_back(posfix, pop(stack));
					view(posfix);
					printf("result: %d#\n", calc(posfix));
					fclose(fp);
					empty(stack);
					empty(posfix);
					return 0;
			}
		}
		printf("%c", ch);
	}
	fclose(fp);
	return 0;
}
