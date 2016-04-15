#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

#define tNULL -1
typedef int TreeType;
typedef struct Tree {
	TreeType value;
	struct Tree * right, *left;
} Tree;

Tree * mom;

Tree * init()
{
	Tree * tree = (Tree*)malloc(sizeof(Tree));
	tree->left = tree->right = NULL;
	tree->value = tNULL;
	return tree;
}
void treePrint(Tree * tree)
{
	printf("%d ", tree->value);
}
void inorder(Tree * tree)
{
	if (tree == NULL)
		return;
	inorder(tree->left);
	treePrint(tree);
	inorder(tree->right);
}
void preorder(Tree * tree)
{
	if (tree == NULL)
		return;
	treePrint(tree);
	preorder(tree->left);
	preorder(tree->right);
}
void postorder(Tree * tree)
{
	if (tree == NULL)
		return;
	postorder(tree->left);
	postorder(tree->right);
	treePrint(tree);
}
void find(Tree * tree, TreeType value)
{
	Tree * now = tree;
	while (now != NULL)
	{
		if (now->value == value)
		{
			printf("%d is in Tree\n", value);
			break;
		}
		else if (now->value < value)
			now = now->right;
		else if (now->value > value)
			now = now->left;
	}
	if (now == NULL)
		printf("%d is not in Tree\n", value);

}
void insert(Tree * tree, TreeType value)
{
	Tree * now = tree, *preleft = tree, *preright = tree;
	while (1)
	{
		if (now != NULL)
		{
			if (now->value == tNULL)
			{
				now->value = value;
				break;
			}
			else if (now->value == value)
			{
				//is already exist
				break;
			}
			else if (now->value > value)
			{
				preleft = now;
				preright = NULL;
				now = now->left;
			}
			else if (now->value < value)
			{
				preleft = NULL;
				preright = now;
				now = now->right;
			}
		}
		else
		{
			now = init();
			now->value = value;
			if (preleft != NULL)
				preleft->left = now;
			if (preright != NULL)
				preright->right = now;
			break;
		}
	}
}
void deleteTree(Tree * tree)
{
	if (tree == NULL)
		return;
	deleteTree(tree->left);
	deleteTree(tree->right);
	free(tree);
}
void deleteValue(Tree * tree, TreeType value)
{
	Tree * now = tree, *preleft = NULL, *preright = mom;
	while (1)
	{
		if (now != NULL)
		{
			if (now->value == value)
			{
				printf("%d is deleted!\n", now->value);
				Tree * ret = now->right, *rret = now;
				if (now->right != NULL)
				{
					for (;ret->left != NULL; rret = ret, ret = ret->left);
					now->value = ret->value;
					if (ret->right != NULL)
						rret->left = ret->right;
					else
					{
						free(rret->right);
						rret->right = NULL;
					}
				}
				else
				{
					if (preleft != NULL)
						preleft->left = now->left;
					if (preright != NULL)
						preright->right = now->left;
					free(now);
				}
				break;
			}
			else if (now->value > value || now->value == tNULL)
			{
				preleft = now;
				preright = NULL;
				now = now->left;
			}
			else if (now->value < value)
			{
				preleft = NULL;
				preright = now;
				now = now->right;
			}
		}
		else
		{
			//can't find
			break;
		}

	}
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

	mom = init();
	Tree * tree = init();
	mom->value = tNULL;
	mom->left = tree;

	char ch;
	int chk, arg;
	do {
		chk = fscanf(fp, "%c", &ch);
		if (chk < 0)
			break;
		switch (ch)
		{
		case 'i':
			fscanf(fp, "%d", &arg);
			insert(tree, arg);
			break;
		case 'd':
			fscanf(fp, "%d", &arg);
			deleteValue(mom, arg);
			break;
		case 'f':
			fscanf(fp, "%d", &arg);
			find(tree, arg);
			break;
		case 'p':
			fscanf(fp, "%c", &ch);
			switch (ch)
			{
			case 'i':
				inorder(tree);
				break;
			case 'r':
				preorder(tree);
				break;
			case 'o':
				postorder(tree);
				break;
			}
			printf("\n");
			break;
		}
	} while (ch != EOF);

	fclose(fp);
	deleteTree(tree);
	return 0;
}
