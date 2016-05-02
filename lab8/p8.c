#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

#define max(a, b) ((a) > (b) ? (a) : (b))

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
int treeHeight(Tree * tree)
{
	if (tree == NULL)
		return -1;
	return 1 + max(treeHeight(tree->left), treeHeight(tree->right));
}

Tree * singleRotateLeft(Tree * tree)
{
	printf("Single Rotate Left: %d\n", tree->value);
	Tree * temp = tree->left;
	tree->left = temp->right;
	temp->right = tree;
	return temp;
}
Tree * singleRotateRight(Tree * tree)
{
	printf("Single Rotate Right: %d\n", tree->value);
	Tree * temp = tree->right;
	tree->right = temp->left;
	temp->left = tree;
	return temp;
}
Tree * doubleRotateLeft(Tree * tree)
{
	printf("Double Rotate Left: %d\n", tree->value);
	tree->left = singleRotateRight(tree->left);
	return singleRotateLeft(tree);
}
Tree * doubleRotateRight(Tree * tree)
{
	printf("Double Rotate Right: %d\n", tree->value);
	tree->right = singleRotateLeft(tree->right);
	return singleRotateRight(tree);
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
Tree * insert(Tree * tree, TreeType value)
{
	if (tree == NULL)
	{
		tree = init();
		tree->value = value;
		tree->left = tree->right = NULL;
	}
	else if (value <= tree->value)
	{
		tree->left = insert(tree->left, value);
		if (treeHeight(tree->left) - treeHeight(tree->right) == 2)
			if (value < tree->left->value)
				tree = singleRotateLeft(tree);
			else
				tree = doubleRotateLeft(tree);
	}
	else if (value >= tree->value)
	{
		tree->right = insert(tree->right, value);
		if (treeHeight(tree->right) - treeHeight(tree->left) == 2)
			if (value > tree->right->value)
				tree = singleRotateRight(tree);
			else
				tree = doubleRotateRight(tree);
	}
	return tree;
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

	Tree * tree = NULL;
	int ch, chk;
	do {
		chk = fscanf(fp, "%d", &ch);
		if (chk < 0)
			break;
		tree = insert(tree, ch);
		inorder(tree);
		printf("\n");
	} while (ch != EOF);
	inorder(tree);
	printf("\n");
	fclose(fp);
	deleteTree(tree);
	return 0;
}
