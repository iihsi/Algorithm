#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BUF_SIZE 80
struct edge {
	int a;
	int b;
	int weight;
	int added;
};
int **matrix;
struct edge *edges;
long counter_UF = 0;
int my_compare(const struct edge *a, const struct edge *b) {
	return (a->weight > b->weight) ? 1 : (a->weight < b->weight) ? -1 : 0;
}
void Disp(int size) {
	int i, j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
}
void Clear(int size) {
	int i;
	for (i = 0; i < size; i++) free(matrix[i]);
	free(matrix);
}
/*** Union-Find functions ***/
void UF_Init(int n);
void UF_Union(int a, int b);
int  UF_Find(int a);
void UF_Clear(int n);
/****************************/
void Kruskal_func(int size, int edge_num) {
	int i, j, k;
	/* Initialize the edge array */
	k = 0;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if (matrix[i][j] > 0) {
				edges[k].weight = matrix[i][j];
				edges[k].added = 0;
				edges[k].a = i; 
				edges[k].b = j; 
				k++;
			}
	/* Sort all edges based on their weight : O(E log E) */
	qsort(edges, edge_num, sizeof(struct edge),
			(int (*)(const void *, const void *))my_compare);
	UF_Init(edge_num);
	for (i = 0; i < edge_num; i++) {
		int a = edges[i].a;
		int b = edges[i].b;
		counter_UF++; /* UF_Find() : O(log E) */
		if (UF_Find(a) != UF_Find(b)/* If Node-a and Node-b are not in the same subset */) {
			/* Join two subsets into a sigle subset */
			/* Edge-i is added to Minimum spanning tree */
			edges[i].added = 1;
            UF_UNION(UF_Find(a), UF_FIND(b));
		}
	}
	for (i = 0; i < edge_num; i++) {
		if (edges[i].added)
			printf("edge(%d,%d) is added\n", edges[i].a, edges[i].b);
	}
	UF_Clear(edge_num);
}
void Kruskal(int size) {
	int i, j, edge_num = 0;
	/* Count the number of edges */
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if (matrix[i][j] > 0) edge_num++;
	/* Allocate memory for the edge array */
	if ((edges = malloc(sizeof(struct edge) * edge_num)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	counter_UF = 0;
	Kruskal_func(size, edge_num);
	printf("Cost = (Num of UF function %ld) * O(log E)\n", counter_UF);
	free(edges);
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int i, j, n;
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
	if ((matrix = malloc(sizeof(int*) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < n; i++) {
		if ((matrix[i] = malloc(sizeof(int) * n)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	}
	/* Initialize the matrix */
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			matrix[i][j] = 0;
	/* Read the graph */
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		int a, b, c;
		sscanf(buf, "%d %d %d", &a, &b, &c);
		matrix[a][b] = c;
		matrix[b][a] = c;
	}
	fclose(fp);

	printf("[d]       Display Graph\n");
	printf("[k]       Kruskal Algorithm\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'k':	Kruskal(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}
/*** Union-Find functions ***/
int *UF_Parent;
int *UF_Rank;
void UF_Init(int n) {
	int i;
	if ((UF_Parent = malloc(sizeof(int) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((UF_Rank = malloc(sizeof(int) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < n; i++) {
		UF_Parent[i] = i;
		UF_Rank[i] = 0;
	}
}
void UF_Union(int a, int b) {
	if (UF_Rank[a] < UF_Rank[b])	UF_Parent[a] = b;
	else				UF_Parent[b] = a;
	if (UF_Rank[a] == UF_Rank[b])	UF_Rank[a]++;
}
int UF_Find(int a) {
	if (UF_Parent[a] == a)	return a;
	else 			return UF_Find(UF_Parent[a]);
}
void UF_Clear(int n) {
	free(UF_Parent);
	free(UF_Rank);
}