#include <stdio.h>
#include <stdlib.h>
struct node {
	int data;
	struct node *left;
	struct node *right;
};
struct node *root;
void Init() {
	root = NULL;
}
int Empty() {
	return (root == NULL);
}
struct node *Insert(struct node *p, int data) {
	if (p == NULL) {
		if ((p = malloc(sizeof(struct node))) == NULL) {
			printf("Memory allocation error\n"); exit(1);
		}
		p->data = data;
		p->left = p->right = NULL;
	} else if (p->data > data)
		p->left = Insert(p->left, data);
	else
		p->right = Insert(p->right, data);
	return p;
}
void Remove(int key) {
	struct node **p, *x;
	p = &root;
	while (1) {
		if (*p == NULL) {
			printf("Not found\n"); return;
		}
		if ((*p)->data == key)
			break; /* Found */
		else if ((*p)->data > key)
			p = &(*p)->left; /* Goto left tree */
		else
			p = &(*p)->right; /* Goto right tree */
	}
	if ((*p)->left == NULL && (*p)->right == NULL) {
		/* Remove a leaf node */
		x = *p;
		*p = NULL;
		free(x);
	} else if ((*p)->left == NULL) {
		/* Remove a node that has a right child */
		x = *p;
		*p = (*p)->right;
		free(x);
	} else if ((*p)->right == NULL) {
		/* Remove a node that has a left child */
		x = *p;
		*p = (*p)->left;
		free(x);
	} else {
		/* Remove a node that has right and left children */
		struct node **left, *n;
		left = &(*p)->left;
		while ((*left)->right != NULL)
			left = &(*left)->right;
		n = *left;
		*left = (*left)->left;
		n->left = (*p)->left;
		n->right = (*p)->right;
		x = *p;
		*p = n;
		free(x);
	}
}
void Clear(struct node *p) {
	if (p != NULL) {
		Clear(p->left);
		Clear(p->right);
		free(p);
	}
}
void Disp(struct node *p) {
	if (p != NULL) {
		Disp(p->left);
		printf("%d ", p->data);
		Disp(p->right);
	}
}
void Disp_Level(struct node *p, int level) {
	if (p != NULL) {
		Disp_Level(p->left, level + 1);
		printf("%d(%d) ", p->data, level);
		Disp_Level(p->right, level + 1);
	}
}
int Find(struct node *p, int key) {
	return 0;
}
int main() {
	int data; char command;
	Init();
	printf("[e]       Empty or Not\n");
	printf("[d]       Display Tree\n");
	printf("[i <num>] Insert Value\n");
	printf("[r <num>] Remove Value\n");
	printf("[f <num>] Find Value\n");
	printf("[b]       Bye\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'e':	printf("Empty %d\n", Empty()); break;
		//case 'd':	Disp(root); printf("\n"); break;
		case 'd':	Disp_Level(root, 0); printf("\n"); break;
		case 'i':	scanf("%d", &data); 
				root = Insert(root, data); break;
		case 'r':	scanf("%d", &data); Remove(data); break;
		case 'f':	scanf("%d", &data); 
				printf("Find %d\n", Find(root, data)); break;
		case 'b':	Clear(root); return 0;
		}
	}
	return 0;
}