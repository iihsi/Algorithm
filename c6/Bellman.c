#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BUF_SIZE 80
struct node {
	int id;
	int weight;
	struct node *next;
};
struct node **list;
int *dist, *prev;
long counter_update = 0;
void Insert(int a, int b, int weight) {
	struct node *p;
	/* Add b to list-a */
	if ((p = malloc(sizeof(struct node))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	p->weight = weight;
	p->id = b;
	p->next = list[a];
	list[a] = p;
	/* Add a to list-b */
	if ((p = malloc(sizeof(struct node))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	p->weight = weight;
	p->id = a;
	p->next = list[b];
	list[b] = p;
}
void Disp(int size) {
	int i;
	struct node *p;
	for (i = 0; i < size; i++) {
		printf("Node %d : ", i);
		for (p = list[i]; p != NULL; p = p->next)
                        printf("%d ", p->id);
                printf("\n");
        }
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
void BellmanFord(int p, int size) {
	int i, u, updated;
	for (i = 0; i < size; i++) {
		dist[i] = INT_MAX;
		prev[i] = -1;
	}
	dist[p] = 0;
	/* For all nodes */
	while (1) {
		updated = 0;
		/* For all edges */
		for (u = 0; u < size; u++) {
			struct node *n;
			for (n = list[u]; n != NULL; n = n->next/* For all adjacent nodes from Node-u */) {
				counter_update++;
				int w = n->weight;
				unsigned newlen = dist[u] + w;
				if (newlen < dist[n->id]/* newlen is shorter than dist[n->id] */) {
					/* Update dist[n->id] */
                    dist[n->id] = newlen;
					/* Update prev[n->id] */
                    prev[n->id] = u;
					updated = 1;
				}
			}
		}
		if (updated == 0) break;
	}
	for (i = 0; i < size; i++) {
		int j;
		if (i != p) {
			printf("%d ", i);
			for (j = prev[i]; j != p; j = prev[j]) printf("%d ", j); 
		}
		printf("%d\n", p);
	}
}
void BellmanFord_All(int size) {
	int i;
	if ((dist = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((prev = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	counter_update = 0;
	for (i = 0; i < size; i++) BellmanFord(i, size);
	printf("Cost = %ld\n", counter_update/size);
	free(dist);
	free(prev);
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

	printf("[d]       Display Graph\n");
	printf("[b]       Bellman-Ford Algorithm\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'b':	BellmanFord_All(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}