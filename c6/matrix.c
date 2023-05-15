#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 80
int **matrix;
int *visit, *queue;
int counter_visit = 0;
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
void DFS(int p, int size) {
	int i;
	/* Node-p is visited */
	visit[p] = 1;
	counter_visit++;
	for (i = 0; i < size; i++) {
		counter_visit++;
		/* If Node-p is connected to Node-i &&
					Node-i is unvisited */
		if (matrix[p][i] == 1 && visit[i] == 0) {
			printf("%d-->%d ", p, i);
			/* Call DFS() from Node-i */
			DFS(i, size);
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
		DFS(i, size);
		printf("\n");
	}
	printf("Visit %d\n", counter_visit / size);
	free(visit);
}
void BFS(int p, int size) {
	int i, pos;
	int head, tail;
	for (i = 0; i < size; i++) visit[i] = 0;
	head = tail = 0;
	/* Node-p is pushed; Node-p is visited */
	queue[tail++] = p; visit[p] = 1;
	do {
		pos = queue[head++]; /* Pop */
		counter_visit++;
		for (i = 0; i < size; i++) {
			counter_visit++;
			/* If Node-pos is connected to Node-i &&
				Node-i is unvisited */
			if (matrix[pos][i] == 1 && visit[i] == 0) {
				printf("%d-->%d ", pos, i);
				/* Node-i is pushed; Node-i is visited */
				queue[tail++] = i; visit[i] = 1;
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