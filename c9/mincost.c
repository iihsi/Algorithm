#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BUF_SIZE 80
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
struct node {
	int id;
	int cap;
	int cost;
	struct node *next;
};
struct node **list;
int *dist, *delta, *prev, *queue;
int **flow;
void Insert(int a, int b, int cap, int cost) {
	struct node *p;
	/* Add b to list-a */
	if ((p = malloc(sizeof(struct node))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	p->cap = cap;	/* capacity for Node-a to Node-b */
	p->cost = cost;	/* cost for Node-a to Node-b */
	p->id = b;
	p->next = list[a];
	list[a] = p;
	/* Add a to list-b */
	if ((p = malloc(sizeof(struct node))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	p->cap = 0;	/* capacity for Node-b to Node-a */
	p->cost = 0;	/* cost for Node-b to Node-a */
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
/*** Heap functions ***/
void PQ_Init(int size, int *key);
void PQ_Clear();
void PQ_Update(int v);
int  PQ_RemoveMin();
int  PQ_Empty();
/**********************/
int Maxflow_Find(int s, int t, int size) {
	int i;
	for (i = 0; i < size; i++) {
		delta[i] = INT_MAX;
		dist[i] = INT_MAX;
		prev[i] = -1;
	}
	dist[s] = 0;
	PQ_Init(size, dist);
	while (!PQ_Empty()) {
		struct node *n;
		int u = PQ_RemoveMin();
		/* If Node-u is sink node, then break */
        if (u == t) break;
		for (n = list[u]; n != NULL; n = n->next/* For all adjacent nodes from Node-u */) {
			int v = n->id;
			if (flow[u][v] < n->cap/* Flow is less than Cap */) {
				int newlen = dist[u] + n->cost; /* dist[u] + Cost */
				if (newlen < dist[v]/* newlen is less than dist[v] */) {
					/* dist[v] is updated */
                    dist[v] = newlen;
					/* prev[v] is updated */
                    prev[v] = u;
					delta[v] = MIN(delta[u], n->cap - flow[u][v]);/* new margin (Cap - Flow) */
					PQ_Update(v);
				}
			}
		}
	}
	return dist[t] != INT_MAX;
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
	int i, totalcost = 0;
	struct node *p;
	for (i = 0; i < size; i++)
		for (p = list[i]; p != NULL; p = p->next)
			if (p->cap > 0) {
                        	printf("%d --> %d (flow %d / cap %d)\n", 
					i, p->id, flow[i][p->id], p->cap);
				totalcost += flow[i][p->id] * p->cost;
			}
	printf("TotalCost = %d\n", totalcost);
}
void FordFulkerson(int size) {
	int i, j;
	int maxflow = 0;
	if ((dist = malloc(sizeof(int) * size)) == NULL) {
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

	free(dist);
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
		int a, b, c, d;
		sscanf(buf, "%d %d %d %d", &a, &b, &c, &d);
		Insert(a, b, c, d);
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
/*** Heap functions ***/
int *Heap;
int *Heap_Index;
int *Heap_Key;
int Heap_Size = 0;
void downheap(int k){
	int j, v;
	v = Heap[k];
	while (k <= Heap_Size / 2) {
		j = k + k;
		if (j < Heap_Size && Heap_Key[Heap[j]] > Heap_Key[Heap[j + 1]])
			j++;
		if (Heap_Key[v] <= Heap_Key[Heap[j]]) break;
		Heap[k] = Heap[j];
		Heap_Index[Heap[j]] = k;
		k = j;
	}
	Heap[k] = v;
	Heap_Index[v] = k;
}
void upheap(int k){
	int v;
	v = Heap[k];
	while (k > 1 && Heap_Key[Heap[k / 2]] >= Heap_Key[v]) {
		Heap[k] = Heap[k / 2];
		Heap_Index[Heap[k / 2]] = k;
		k = k / 2;
	}
	Heap[k] = v;
	Heap_Index[v] = k;
}
void PQ_Init(int size, int *key) {
	int i;
	Heap_Size = size;
	Heap_Key = key;
	if ((Heap = malloc(sizeof(int) * (Heap_Size + 1))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((Heap_Index = malloc(sizeof(int) * Heap_Size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < Heap_Size; i++) {
            Heap[i + 1] = i;
            Heap_Index[i] = i + 1;
        }
        for (i = Heap_Size / 2; i >= 1; i--) downheap(i);
}
void PQ_Clear() {
	free(Heap);
	free(Heap_Index);
}
int PQ_Empty() {
	return (Heap_Size == 0);
}
int PQ_RemoveMin() {
	int v = Heap[1];
	Heap[1] = Heap[Heap_Size];
	Heap_Index[Heap[Heap_Size]] = 1;
	Heap_Size--;
	downheap(1);
        return v;
}
void PQ_Update(int v) {
	upheap(Heap_Index[v]);
}
void PQ_Disp(int size) {
	int i;
	for (i = 1; i <= Heap_Size; i++) printf("%d ", Heap[i]);
	printf("\n");
}