#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BUF_SIZE 80
int **matrix;
int *dist, *prev, *visited;
long counter_update = 0;
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
void DijkstraMatrix(int p, int size) {
	int i;
	for (i = 0; i < size; i++) {
		dist[i] = INT_MAX;
		prev[i] = -1;
		visited[i] = 0;
	}
	dist[p] = 0;
	while (1) {
		int u = -1;
		int min = INT_MAX;
		for (i = 0; i < size; i++) {
			counter_update++;
			if (visited[i] == 0 && dist[i] < min/* Node-i is unvisited && dist[i] less than min*/) {
				min = dist[i];
				u = i;
			}
		}
		if (u == -1) break;
		visited[u] = 1;
		for (i = 0; i < size; i++) {
			if (matrix[u][i] > 0) {
				counter_update++;
				int w = matrix[u][i]; /* Cost from Node-u to Node-i */
				int newlen = dist[u] + w; /* Cost from Node-p to Node-u +w */
				if (newlen < dist[i]/* newlen is shorter than dist[n->id] */) {
					/* Update dist[i] */
                    dist[i] = newlen;
					/* Update prev[i] */
                    prev[i] = u;
				} 
			}
		}
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
void DijkstraMatrix_All(int size) {
	int i;
	if ((dist = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((prev = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((visited = malloc(sizeof(int) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	counter_update = 0;
	for (i = 0; i < size; i++) DijkstraMatrix(i, size);
	printf("Cost = %ld\n", counter_update/size);
	free(dist);
	free(prev);
	free(visited);
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
	printf("[m]       Dijkstra Algorithm (Matrix)\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'm':	DijkstraMatrix_All(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}