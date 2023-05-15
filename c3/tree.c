#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 80
#define NAME_SIZE 32
struct student {
	int score;
	char name[NAME_SIZE];
	struct student *left;
	struct student *right;
};
int counter_compare, counter_copy = 0;
struct student *data;
struct student *root = NULL;
int my_compare(const struct student *a, const struct student *b) {
	counter_compare++;
	return strcmp(a->name, b->name);
}
void my_copy(struct student *src, struct student *dst) {
	counter_copy++;
	memcpy(dst, src, sizeof(struct student));
}
struct student *Insert(struct student *p, struct student *d) {
    if (p == NULL) {
        p = malloc(sizeof(struct student));
        my_copy(d, p);
        p->left = p->right = NULL;
    }
    else if (my_compare(p, d) > 0)
        p->left = Insert(p->left, d);
    else
    	p->right = Insert(p->right, d);
	return NULL;
}
int Search(struct student *p, struct student *d) {
	int val;
	if (p == NULL) return 0;    /* Not found */
	val = my_compare(p, d);
	if (val == 0)     return 1; /* Found */
	else if (val > 0) return Search(p->left, d); /* Search() that searches  left half */
	else              return Search(p->right, d); /* Search() that searches right half */
}
void Remove(struct student *d) {
	int val;
	struct student **p, *x;
	p = &root;
	while (1) {
		if (*p == NULL) {
			printf("Not found\n"); return;
		}
		val = my_compare(*p, d);
		if (val == 0)
			break; /* Found */
		else if (val > 0)
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
		struct student **left, *n;
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
void Disp(struct student *p) {
	if (p != NULL) {
		Disp(p->left);
		printf("%d\t%s\n", p->score, p->name);
		Disp(p->right);
	}
}
void All_Insert(int size) {
	int i;
	counter_compare = 0; counter_copy = 0;
	for (i = 0; i < size; i++) root = Insert(root, &data[i]);
	printf("Compare %d Copy %d\n", counter_compare/size, counter_copy/size);
}
void All_Search(int size) {
	int i;
	counter_compare = 0; counter_copy = 0;
	for (i = 0; i < size; i++) Search(root, &data[i]);
	printf("Compare %d Copy %d\n", counter_compare/size, counter_copy/size);
}
void All_Remove(int size) {
	int i;
	counter_compare = 0; counter_copy = 0;
	for (i = 0; i < size; i++) Remove(&data[i]);
	printf("Compare %d Copy %d\n", counter_compare/size, counter_copy/size);
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int n;
	char buf[BUF_SIZE];
	char command;
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]); exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file (%s) \n", argv[1]); exit(1);
	}
	/* Read the file to count the number of lines in the file */
	n = 0;
	while (fgets(buf, BUF_SIZE, fp) != NULL)
		n++;
	/* Memory allocation */
	if ((data = malloc(sizeof(struct student) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Read the file again to copy the data */
	n = 0;
	fseek(fp, 0L, SEEK_SET);
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		sscanf(buf, "%d\t%s", &data[n].score, data[n].name);
		n++;
	}
	fclose(fp);

	printf("[d]       Display Table\n");
	printf("[i]       All Insert\n");
	printf("[s]       All Search\n");
	printf("[r]       All Remove\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(root); break;
		case 'i':	All_Insert(n); break;
		case 's':	All_Search(n); break; 
		case 'r':	All_Remove(n); break;
		case 'e':	free(data); return 0;
		}
	}
	return 0;
}