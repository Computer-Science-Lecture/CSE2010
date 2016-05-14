#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) && defined(_MSC_VER)
#pragma warning (disable : 4996)
#endif

enum { RED, BLACK };

typedef int COLOR;
typedef int valueType;
typedef struct Node {
	valueType value;
	COLOR color;
	struct Node *right, *left, *parent;
}*node;
typedef node* tree;

node initilize(node, valueType);
node grandparent(node);
node uncle(node);
node sibling(node);

node findNode(tree, valueType);

void insertNode(tree, valueType);
void insertUtil(node);

void removeNode(tree, valueType);
void removeUtil(node);

void rotateRight(node);
void rotateLeft(node);

node findNode(tree, valueType);
void replaceNode(tree, node, node);

void printNode(node n);
void inorderIterator(node n, void(*func)(node));
void inorderPrint(tree t);

void destroy(node);

node initilize(node p, valueType v)
{
	node tree = (node)malloc(sizeof(struct Node));
	tree->left = tree->right = NULL;
	tree->parent = p;
	tree->value = v;
	tree->color = RED;
	return tree;
}
node grandparent(node n)
{
	if (n == NULL || n->parent == NULL)
		return NULL;
	return n->parent->parent;
}
node uncle(node n)
{
	node g = grandparent(n);
	if (n == NULL || g == NULL)
		return NULL;
	if (n->parent == g->left)
		return g->right;
	else
		return g->left;
}
node sibling(node n)
{
	if (n == n->parent->left)
		return n->parent->right;
	else
		return n->parent->left;
}
int colorOf(node n)
{
	return n == NULL ? BLACK : n->color;
}
void insertNode(tree t, valueType v)
{
	int pl = 0;
	node ptr, btr = NULL, newNode;

	for (ptr = *t; ptr != NULL;
	btr = ptr, ptr = ((pl = (ptr->value > v)) ? ptr->left : ptr->right));

		newNode = initilize(btr, v);

	if (btr != NULL)
		(pl) ? (btr->left = newNode) : (btr->right = newNode);

	insertUtil(newNode);

	ptr = newNode;
	for (ptr = newNode; ptr != NULL; btr = ptr, ptr = ptr->parent);
	*t = btr;
}
void insertUtil(node n)
{
	node u = uncle(n), g = grandparent(n), p = n->parent;
	if (p == NULL)
		n->color = BLACK;
	else if (p->color == BLACK)
		return;
	else if (u != NULL && u->color == RED)
	{
		p->color = BLACK;
		u->color = BLACK;
		g->color = RED;

		insertUtil(g);
	}
	else
	{
		if (n == p->right && p == g->left)
		{
			rotateLeft(p);
			n = n->left;
		}
		else if (n == p->left && p == g->right)
		{
			rotateRight(p);
			n = n->right;
		}

		g = grandparent(n);
		p = n->parent;

		p->color = BLACK;
		g->color = RED;

		if (n == p->left)
			rotateRight(g);
		else
			rotateLeft(g);
	}
}
void replaceNode(tree t, node o, node n)
{
	if (o->parent == NULL)
		*t = n;
	else
	{
		if (o == o->parent->left)
			o->parent->left = n;
		else
			o->parent->right = n;
	}

	if (n != NULL)
		n->parent = o->parent;
}
void removeNode(tree t, valueType v)
{
	node n = findNode(t, v), c;

	if (n == NULL)
		return;
	if (n->left != NULL && n->right != NULL)
	{
		node pred = n->left;
		while (pred->right != NULL)
			pred = pred->right;
		n->value = pred->value;
		n = pred;
	}

	c = n->right == NULL ? n->left : n->right;
	if (n->color == BLACK)
	{
		n->color = colorOf(c);
		removeUtil(n);
	}

	replaceNode(t, n, c);
	free(n);
}
void removeUtil(node n)
{
	if (n->parent == NULL)
		return;

	node s = sibling(n);
	if (colorOf(s) == RED)
	{
		n->parent->color = RED;
		s->color = BLACK;
		if (n == n->parent->left)
			rotateLeft(n->parent);
		else
			rotateRight(n->parent);
	}

	s = sibling(n);
	if (colorOf(n->parent) == BLACK && colorOf(s) == BLACK &&
		colorOf(s->left) == BLACK && colorOf(s->right) == BLACK)
	{
		s->color = RED;
		removeUtil(n->parent);
	}
	else if (colorOf(n->parent) == RED && colorOf(s) == BLACK &&
		colorOf(s->left) == BLACK && colorOf(s->right) == BLACK)
	{
		s->color = RED;
		n->parent->color = BLACK;
	}
	else
	{
		if (n == n->parent->left && colorOf(s) == BLACK &&
			colorOf(s->left) == RED && colorOf(s->right) == BLACK)
		{
			s->color = RED;
			s->left->color = BLACK;
			rotateRight(s);
		}
		else if (n == n->parent->right && colorOf(s) == BLACK &&
			colorOf(s->right) == RED && colorOf(s->left) == BLACK)
		{
			s->color = RED;
			s->right->color = BLACK;
			rotateLeft(s);
		}

		s->color = colorOf(n->parent);
		n->parent->color = BLACK;
		if (n == n->parent->left)
		{
			s->right->color = BLACK;
			rotateLeft(n->parent);
		}
		else
		{
			s->left->color = BLACK;
			rotateRight(n->parent);
		}
	}
}

void rotateRight(node tree)
{
	printf("rotate Right %d\n", tree->value);
	node c = tree->left;
	node p = tree->parent;

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
}
void rotateLeft(node tree)
{
	printf("rotate Left %d\n", tree->value);
	node c = tree->right;
	node p = tree->parent;

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
}

node findNode(tree t, valueType v)
{
	node p;
	for (p = *t; p != NULL && p->value != v; p = (p->value > v ? p->left : p->right));
	return p;
}
void printNode(node n)
{
	printf("%d(%s) ", n->value, (n->color == BLACK ? "b" : "r"));
}
void inorderIterator(node n, void(*func)(node))
{
	if (n == NULL)
		return;
	inorderIterator(n->left, func);
	func(n);
	inorderIterator(n->right, func);
}
void inorderPrint(tree t)
{
	inorderIterator(*t, printNode);
	printf("\n");
}
void destroy(node tree)
{
	if (tree == NULL)
		return;
	destroy(tree->left);
	destroy(tree->right);
	free(tree);
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

	node tree = NULL;

	char c = 0, cc = 0;
	for (int ch = 0, chk = 0; !(chk < 0); inorderPrint(&tree))
	{
		if (c == '\n')
		{
			printf("start delete\n");
			cc = 1;
		}

		chk = fscanf(fp, "%d", &ch);
		chk = fscanf(fp, "%c", &c);

		if (!cc)
			insertNode(&tree, ch);
		else
			removeNode(&tree, ch);
	}

#if defined(_WIN32) && defined(_MSC_VER)
	system("pause");
#endif

	destroy(tree);
	fclose(fp);

	return 0;
}
