#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#define BUF_SIZE 80
struct node {
	double NS;	/* latitude */
	double EW;	/* longitude */
};
struct node *nodes;
double **dist;
double **table;
int **next;
long all_visited(int size) { return (1 << size) - 1; }
int is_visited(long S, int pos) { return (S >> pos) & 1; }
long set_visited(long S, int pos) { return (1 << pos) | S; }
void Disp(int size) {
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if (i != j)
				printf("x%d_%d = %f;\n", i, j, dist[i][j]);
}
void Clear(int size) {
	int i;
	for (i = 0; i < size; i++) free(dist[i]);
	free(dist); free(nodes);
}
void PrintPath(int **next, int size) {
	long S = 0;
	int i = 0;
	printf("%2d ", i);
	while (S != all_visited(size)) {
		i = next[S][i];
		S = set_visited(S, i);
		printf("%2d ", i);
	}
	printf("\n");
}
void DP(int size) {
	long s, e;
	s = clock();
	long S;
	int i, j;
	/* Allocate memory for dynamic programming */
	if ((table = malloc(sizeof(double*) * (1 << size))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((next = malloc(sizeof(int*) * (1 << size))) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (S = 0; S < (1 << size); S++) {
		if ((table[S] = malloc(sizeof(double) * size)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
		if ((next[S] = malloc(sizeof(int) * size)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	}
	/* Initialize the memory for dynamic programming */
	for (S = 0; S < (1 << size); S++)
		for (i = 0; i < size; i++) {
			table[S][i] = DBL_MAX;
			next[S][i] = -1;
		}
	table[all_visited(size)][0] = 0;
	/* Dynamic programming */
	for (S = all_visited(size); S >= 0; S--)
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++)
				if (!is_visited(S, j)) {
					double newlen = dist[i][j] + table[set_visited(S, j)][j];
					if (newlen < table[S][i]) {
						table[S][i] = newlen;
						next[S][i] = j;
					}
				}
	/* Result */
	e = clock();
	printf("time = %d\n", e - s);
	printf("Minimum Cost = %f\n", table[0][0]);
	PrintPath(next, size);
	/* Release the memory for dynamic programming */
	for (S = 0; S < (1 << size); S++) {
		free(table[S]); free(next[S]);
	}
	free(table); free(next);
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
	/* Read the file to check the problem size */
	fgets(buf, BUF_SIZE, fp);
	sscanf(buf, "%d", &n);

	/* Memory allocation for the input data */
	if ((nodes = malloc(sizeof(struct node) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Read the problem */
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		int a; char b[16]; double c; double d;
		sscanf(buf, "%d %s %lf %lf", &a, b, &c, &d);
		nodes[a].NS = c;	/* latitude of Node-a */
		nodes[a].EW = d;	/* longitude of Node-a */
	}
	fclose(fp);

	if ((dist = malloc(sizeof(double*) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < n; i++) {
		if ((dist[i] = malloc(sizeof(double) * n)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
		/* Distance from Node-i to Node-j */
		for (j = 0; j < n; j++) {
			if (nodes[i].NS > nodes[j].NS)
				dist[i][j] = nodes[i].NS - nodes[j].NS;
			else
				dist[i][j] = nodes[j].NS - nodes[i].NS;
			if (nodes[i].EW > nodes[j].EW)
				dist[i][j] += nodes[i].EW - nodes[j].EW;
			else
				dist[i][j] += nodes[j].EW - nodes[i].EW;
		}
	}

	printf("[d]       Display TSP Problem\n");
	printf("[p]       Dynamic Programming Search\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'p':	DP(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}