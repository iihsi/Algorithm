#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define BUF_SIZE 80
int **matrix;
int **dist, **prev;
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
void FloydWarshall_func(int size) {
	int i, j, k;
	/* Initialization */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			dist[i][j] = (matrix[i][j]==0) ? INT_MAX : matrix[i][j];
			prev[i][j] = (matrix[i][j]==0) ? -1 : i;
		}
		dist[i][i] = 0;
	}
	/* Dynamic programming */
	for (k = 0; k < size; k++) {
		for (i = 0; i < size; i++) {
			for (j = 0; j < size; j++) {
				unsigned newlen = dist[i][k] + dist[k][j]; /* Dist(i,k) + Dist(k,j) */
				if (newlen < dist[i][j]/* newlen is shorter than dist[i][j] */) {
					/* Update dist[i][j] */
                    dist[i][j] = newlen;
					/* Update prev[i][j] */
                    prev[i][j] = prev[k][j];
				}
			}
		}
	}
	/* Print out the results */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (dist[i][j] != INT_MAX && i != j) {
				printf("%d ", j);
				for (k = prev[i][j]; k != i; k = prev[i][k])
					printf("%d ", k); 
			}
			printf("%d\n", i);
		}
	}
}
void FloydWarshall(int size) {
	int i;
	if ((dist = malloc(sizeof(int*) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < size; i++) {
		if ((dist[i] = malloc(sizeof(int) * size)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	}
	if ((prev = malloc(sizeof(int*) * size)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < size; i++) {
		if ((prev[i] = malloc(sizeof(int) * size)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	}
	FloydWarshall_func(size);
	for (i = 0; i < size; i++) free(dist[i]);
	free(dist);
	for (i = 0; i < size; i++) free(prev[i]);
	free(prev);
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
	printf("[f]       Floyd-Warshall Algorithm\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'f':	FloydWarshall(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}