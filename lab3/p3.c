#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;
struct Node
{
	ElementType value;
	Position next;
};

List makeList();
void makeEmpty(List);
int isEmpty(List);
int isLast(List, Position);

Position Create(ElementType);
void Insert(List, Position, Position);
void Delete(List, ElementType);
void DelList(List); 

Position findPrev(List, ElementType);
Position find(List, ElementType);

int main(int argc, char * argv[])
{
	FILE * fp;
	char command;
	int arg, tar;
	List list = makeList();

	if (argc < 2)
	{
		printf("you must input argument.\n");
		printf("%s [input]\n", argv[0]);
		return -1;
	}
	
	fp = fopen(argv[1], "r");

	while (1)
	{
		do {
			fscanf(fp, "%c", &command);
		} while (command == '\n' || command == ' ');
		switch (command)
		{
		case 'i':
			fscanf(fp, "%d %d", &arg, &tar);
			if (tar == -1)
				Insert(list, list, Create(arg));
			else
			{
				Position node = find(list, tar);
				if (node == NULL)
					printf("Insertion(%d) Failed : element %d is not in the list\n", arg, tar);			
				else
					Insert(list, node, Create(arg));
			}
			break;
		case 'd':
			fscanf(fp, "%d", &arg);
			Delete(list, arg);
			break;
		case 'f':
		{
			fscanf(fp, "%d", &arg);
			Position obj = findPrev(list, arg);
			if (obj != NULL)
				printf("Key of the previous node of %d is %d\n", arg, obj->value );
			else
				printf("Could not find %d in the list\n", arg);
			break;
		}
			
		case 'p':
			break;
		default:
			printf("There are no command.\n");
		}
		if (command == 'p')
			break;
	}

	Position ptr;
	for (ptr = list->next; ptr != NULL; ptr = ptr->next)
		printf("key: %d\t", ptr->value);
	printf("\n");

}

List makeList()
{
	List list = (List)malloc(sizeof(List));
	list->value = -1;
	list->next = NULL;
	return list;
}
void makeEmpty(List list)
{
	list = (List)malloc(sizeof(List));
	list->next = NULL;
}
int isEmpty(List list)
{
	if (list->next == NULL)
		return 1;
	return 0;
}
int isLast(List list, Position pos)
{
	if (pos->next == NULL)
		return 1;
	return 0;
}

Position Create(ElementType value)
{
	Position node = (Position)malloc(sizeof(List));
	node->next = NULL;
	node->value = value;
	return node;
}
void Insert(List list, Position tar, Position obj)
{
	if (tar == NULL)
	{
		printf("Could not find %d in the list\n", obj->value);
		return;
	}
	List ptr = list;
	for (; ptr != tar; ptr = ptr->next);
	Position temp = ptr->next;
	ptr->next = obj;
	obj->next = temp;
}
void Delete(List list, ElementType v)
{
	Position tar = find(list, v);
	if (tar == NULL)
	{
		printf("Deletion failed : element %d is not in the list\n", v);
		return;
	}
	findPrev(list, v)->next = tar->next;
	free(tar);
}
void DelList(List list)
{
	Position ptr;
	for (ptr = list; ptr != NULL;)
	{
		Position pts = ptr;
		ptr = ptr->next;
		free(pts);
	}
}

Position findPrev(List list, ElementType v)
{
	Position ptr ,pts;
	for (ptr = list, pts = NULL; ptr != NULL; pts = ptr, ptr = ptr->next)
		if (ptr->value == v)
			return pts;
	return NULL;
}
Position find(List list, ElementType v)
{
	Position ptr;
	for (ptr = list; ptr != NULL; ptr = ptr->next)
		if (ptr->value == v)
			return ptr;
	return NULL;
}
