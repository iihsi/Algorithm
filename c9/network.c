#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BUF_SIZE 80
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
struct node {
	int id;
	int cap;
	struct node *next;
};
struct node **list;
void Insert(int a, int b, int cap) {
	struct node *p;
	/* Add b to list-a */
	if ((p = malloc(sizeof(struct node))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	p->cap = cap;	/* capacity for Node-a to Node-b */
	p->id = b;
	p->next = list[a];
	list[a] = p;
	/* Add a to list-b */
	if ((p = malloc(sizeof(struct node))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	p->cap = 0;	/* capacity for Node-b to Node-a */
	p->id = a;
	p->next = list[b];
	list[b] = p;
}
void Disp(int size) {
	int i;
	struct node *p;
	for (i = 0; i < size; i++)
		for (p = list[i]; p != NULL; p = p->next)
			if (p->cap > 0)
                        	printf("%d --> %d (cap %d)\n", i, p->id, p->cap);
}
void Clear(int size) {
	int i;
	struct node *p, *q;
	for (i = 0; i < size; i++) {
		for (q = list[i], p = q->next; p != NULL; q = p, p = p->next) {
			q->next = p->next;
			free(p);
		}
		if (list[i]) free(list[i]);
        }
	free(list);
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int i, n;
	char buf[BUF_SIZE];
	char command;
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]); exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file (%s) \n", argv[1]); exit(1);
	}
	/* Read the file to check the graph size */
	fgets(buf, BUF_SIZE, fp);
	sscanf(buf, "%d", &n);

	/* Memory allocation for the input graph */
	if ((list = malloc(sizeof(struct node*) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < n; i++)
		list[i] = NULL;
	/* Read the graph */
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		int a, b, c;
		sscanf(buf, "%d %d %d", &a, &b, &c);
		Insert(a, b, c);
	}
	fclose(fp);

	printf("[d]       Display Network Flow\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}