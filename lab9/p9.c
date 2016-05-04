#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

enum  COLOR { RED, BLACK };

#define max(a, b) ((a) > (b) ? (a) : (b))
#define tNULL -1
typedef int TreeType;
typedef int Color;
typedef struct Tree {
	TreeType value;
	Color color;
	struct Tree *right, *left, *parent;
} Tree;

void insert_1(Tree *);
void insert_2(Tree *);
void insert_3(Tree *);
void insert_4(Tree *);
void insert_5(Tree *);
void singleRotateRight(Tree *);
void singleRotateLeft(Tree *);

Tree * init()
{
	Tree * tree = (Tree*)malloc(sizeof(Tree));
	tree->left = tree->right = NULL;
	tree->value = tNULL;
	return tree;
}
Tree * RBgrandparent(Tree * tree)
{
	return tree->parent->parent;
}
Tree * RBuncle(Tree * tree)
{
	if (tree->parent == RBgrandparent(tree)->left)
		return RBgrandparent(tree)->right;
	else
		return RBgrandparent(tree)->left;
}
void insert_1(Tree * tree)
{
	if (tree->parent == NULL)
		tree->color = BLACK;
	else
		insert_2(tree);
}
void insert_2(Tree * tree)
{
	if (tree->parent->color == BLACK)
		return;
	else
		insert_3(tree);
}
void insert_3(Tree * tree)
{
	if (RBuncle(tree) != NULL && RBuncle(tree)->color == RED)
	{
		tree->parent->color = BLACK;
		RBuncle(tree)->color = BLACK;
		RBgrandparent(tree)->color = RED;

		insert_1(RBgrandparent(tree));
	}
	else
		insert_4(tree);
}
void insert_4(Tree * tree)
{
	if (tree == tree->parent->right &&
		tree->parent == RBgrandparent(tree)->left)
	{
		singleRotateLeft(tree->parent);
		tree = tree->left;
	}
	else if (tree == tree->parent->left &&
		tree->parent == RBgrandparent(tree)->right)
	{
		singleRotateRight(tree->parent);
		tree = tree->right;
	}
	insert_5(tree);
}
void insert_5(Tree * tree)
{	
	tree->parent->color = BLACK;
	RBgrandparent(tree)->color = RED;

	if (tree == tree->parent->left)
		singleRotateRight(RBgrandparent(tree));
	else
		singleRotateLeft(RBgrandparent(tree));
}

void treePrint(Tree * tree)
{
	printf("%d(%s) ", tree->value, (tree->color == BLACK ? "b" : "r") );
}
void inorder(Tree * tree)
{
	if (tree == NULL)
		return;
	inorder(tree->left);
	treePrint(tree);
	inorder(tree->right);
}

void singleRotateLeft(Tree * tree)
{
	Tree * c = tree->right;
	Tree * p = tree->parent;

	if (c->left != NULL)
		c->left->parent = tree;

	tree->right = c->left;
	tree->parent = c;
	c->left = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->left == tree)
			p->left = c;
		else
			p->right = c;
	}
	printf("rotation %d, left\n", tree->value);
}
void singleRotateRight(Tree * tree)
{
	Tree * c = tree->left;
	Tree * p = tree->parent;

	if (c->right != NULL)
		c->right->parent = tree;

	tree->left = c->right;
	tree->parent = c;
	c->right = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->right == tree)
			p->right = c;
		else
			p->left = c;
	}
	printf("rotation %d, right\n", tree->value);
}
void insert(Tree ** tree, TreeType value)
{
	int left = 0;
	Tree  * ptr = *tree, *btr = NULL,  *newNode = (Tree*)malloc(sizeof(Tree));
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->color = RED;
	newNode->parent = NULL;
	newNode->value = value;

	for (;;)
	{
		if (ptr == NULL)
		{
			newNode->parent = btr;
			if (btr != NULL)
			{
				if (left)
					btr->left = newNode;
				else
					btr->right = newNode;
			}
			insert_1(newNode);
			
			ptr = newNode;
			while (ptr != NULL)
			{
				btr = ptr;
				ptr = ptr->parent;
			}

			*tree = btr;
			break;
		}
		else
		{
			btr = ptr;
			if (ptr->value < value)
			{
				left = 0;
				ptr = ptr->right;
			}
			else
			{
				left = 1;
				ptr = ptr->left;
			}
		}
	}
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
void deleteTree(Tree * tree)
{
	if (tree == NULL)
		return;
	deleteTree(tree->left);
	deleteTree(tree->right);
	free(tree);
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
			insert(&tree, ch);
		inorder(tree);
		printf("\n");
	} while (ch != EOF);
	inorder(tree);
	printf("\n");
	fclose(fp);
	deleteTree(tree);
	return 0;
}
