#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 80
struct node {
	int id;
	int weight;
	struct node *next;
};
struct node **list;
int *visit, *queue;
int counter_visit = 0;
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
	/* Fill out the code */
	p->weight = weight;
    p->id = a; p->next = list[b]; list[b] = p;
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
void DFS(int p) {
	struct node *n;
	visit[p] = 1;
	counter_visit++;
	/* For all adjacent nodes from p */
	for (n = list[p];n != NULL; n = n->next) {
		counter_visit++;
		/* If the adjacent node is unvisited */
		if (visit[n->id] == 0) {
			printf("%d-->%d ", p, n->id);
			/* Call DFS from the adjacent node */
			DFS(n->id);
		}
	}
}
void DFS_All(int size) {
	int i, j;
	if ((visit = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	counter_visit = 0;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) visit[j] = 0;
		DFS(i);
		printf("\n");
	}
	printf("Visit %d\n", counter_visit / size);
	free(visit);
}
void BFS(int p, int size) {
	struct node *n;
	int i, pos;
	int head, tail;
	for (i = 0; i < size; i++) visit[i] = 0;
	head = tail = 0;
	queue[tail++] = p; visit[p] = 1;
	do {
		pos = queue[head++];
		counter_visit++;
		/* For all adjacent nodes from pos */
		for (n = list[pos]; n != NULL; n = n->next) {
			counter_visit++;
			/* If the adjacent node is unvisited */
			if (visit[n->id] == 0) {
				printf("%d-->%d ", pos, n->id);
				queue[tail++] = n->id; visit[n->id] = 1;
			}
		}
	} while (head != tail);
}
void BFS_All(int size) {
	int i;
	if ((visit = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((queue = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	counter_visit = 0;
	for (i = 0; i < size; i++) {
		BFS(i, size);
		printf("\n");
	}
	printf("Visit %d\n", counter_visit / size);
	free(visit);
	free(queue);
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
	printf("[s]       Depth First Search (DFS)\n");
	printf("[b]       Breadth First Search (BFS)\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 's':	DFS_All(n); break;
		case 'b':	BFS_All(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}