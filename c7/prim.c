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
int *key, *prev, *visited;
long counter_heap = 0;
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
/*** Heap functions ***/
void PQ_Init(int size, int *key);
void PQ_Clear();
void PQ_Update(int v);
int  PQ_RemoveMin();
int  PQ_Empty();
/**********************/
void PrimPQ_func(int p, int size) {
	int i;
	for (i = 0; i < size; i++) {
		key[i] = INT_MAX;
		prev[i] = -1;
		visited[i] = 0;
	}
	key[p] = 0;
	PQ_Init(size, key); /* O(V log V) */
	counter_heap += size; /* Cost += size * O(log V) */
	while (!PQ_Empty()) {
		struct node *n;
		int u = PQ_RemoveMin();
		visited[u] = 1;
		/* For all edges --> E times */
		for (n = list[u]; n != NULL; n = n->next/* For all adjacent nodes from Node-u */) {
			int v = n->id;
			int w = n->weight;
			if (visited[v] == 0 && w < key[v]/* Node-v is unvisted && w is less than key[v] */) {
				/* Update key[v]  */
                key[v] = w;
				/* Update prev[v] */
                prev[v] = u;
				PQ_Update(v); /* O(log V) */
			}
			counter_heap++; /* Cost += O(log V) */
		}
	}
	for (i = 0; i < size; i++) {
		int j;
		if (i != p) {
			printf("%d ", i);
			for (j = prev[i]; j != p; j = prev[j]) printf("%d ",j); 
		}
		printf("%d\n", p);
	}
	PQ_Clear();
}
void PrimPQ(int size) {
	int root = 0; /* Root node (you can select any node) */
	if ((key = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((prev = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((visited = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	counter_heap = 0;
	PrimPQ_func(root, size);
	printf("Cost = (Num of heap update %ld) * O(log V)\n", counter_heap);
	free(key);
	free(prev);
	free(visited);
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
	printf("[p]       Prim Algorithm (Priority Queue Version)\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'p':	PrimPQ(n); break;
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