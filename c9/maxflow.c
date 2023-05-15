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
int *visited, *delta, *prev, *queue;
int **flow;
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
int Maxflow_Find(int s, int t, int size) {
	struct node *n;
	int i, u, reached = 0;
	int head, tail;
	for (i = 0; i < size; i++) {
		delta[i] = INT_MAX;
		prev[i] = -1;
		visited[i] = 0;
	}
	head = tail = 0;
	/* Node-s is pushed and Node-s is visited */
    queue[tail++] = s; visited[s] = 1;
	do {
		u = queue[head++]; /* Pop */
		for (n = list[u]; n != NULL; n = n->next/* For all adjacent nodes from Node-u */) {
			int v = n->id;
			if (visited[v] == 0 && flow[u][v] < n->cap/* Node-v is unvisited && Flow is less than Cap*/) {
				/* Node-v is pushed */
                queue[tail++] = v;
				/* Node-v is visited */
                visited[v] = 1;
				/* prev[v] is updated */
                prev[v] = u;
				delta[v] = MIN(delta[u], n->cap - flow[u][v]/* new margin (Cap - Flow) */);
				if (v == t) reached = 1;
			}
		}
	} while (head != tail && reached == 0);
	return reached;
}
int Maxflow_Update(int s, int t, int size) {
	int v = t;
	do {
		/* Update flow from Node-prev[v] to Node-v */
        flow[prev[v]][v] += delta[t];
		/* Update flow from Node-v to Node-prev[v] */
        flow[v][prev[v]] -= delta[t];
		v = prev[v];
	} while (v != s);
	return delta[t];
}
void Maxflow_Result(int size) {
	int i;
	struct node *p;
	for (i = 0; i < size; i++)
		for (p = list[i]; p != NULL; p = p->next)
			if (p->cap > 0)
                        	printf("%d --> %d (flow %d / cap %d)\n", 
					i, p->id, flow[i][p->id], p->cap);
}
void FordFulkerson(int size) {
	int i, j;
	int maxflow = 0;
	if ((visited = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((delta = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((prev = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((queue = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((flow = malloc(sizeof(int*) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < size; i++) {
		if ((flow[i] = malloc(sizeof(int) * size)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
		for (j = 0; j < size; j++) flow[i][j] = 0;
	}

	/* source node: 0, sink node: size-1 */
	while (Maxflow_Find(0, size - 1, size))
		maxflow += Maxflow_Update(0, size - 1, size); 
	Maxflow_Result(size);
	printf("MaxFlow = %d\n", maxflow);

	free(visited);
	free(delta);
	free(prev);
	free(queue);
	for (i = 0; i < size; i++) free(flow[i]);
	free(flow);
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
	printf("[f]       Ford-Fulkerson Algorithm (BFS)\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'f':	FordFulkerson(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}