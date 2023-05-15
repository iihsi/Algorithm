#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define BUF_SIZE 80
#define QUEUE_SIZE 20	/* TabuList size (tuning needed) */
#define LOOP_MAX 2000	/* Num of iterations of the main loop (tuning needed) */
struct node {
	double NS;	/* latitude */
	double EW;	/* longitude */
};
struct node *nodes;
double **dist;
int *path;
/*** Queue Functions ***/
void Queue_Init();
int  Queue_Find(double data);
void Queue_PushBack(double data);
/***********************/
void PathSwap(int a, int b) {
	int tmp = path[a]; path[a] = path[b]; path[b] = tmp;
}
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
	free(dist); free(nodes); free(path);
}
double EvaluatePath(int size) {
	int i;
	double cost = 0;
	for (i = 1; i < size; i++) cost += dist[path[i - 1]][path[i]];
	cost += dist[path[size - 1]][path[0]];
	return cost;
}
void PrintPath(int size) {
	int i;
	for (i = 0; i < size; i++) printf("%2d ", path[i]);
	printf("%2d : Cost = %f\n", path[0], EvaluatePath(size));
}
void InitialSolution(int size) {
	int i;
	srand((unsigned int)time(NULL));
	for (i = 0; i < size; i++) path[i] = i;
	/* Swap two nodes randomly */
	for (i = 0; i < size; i++) PathSwap(rand() % size, rand() % size);
}
void TabuSearch(int size) {
	long s, e;
	s = clock();
	int i, j;
	Queue_Init();
	for (i = 0; i < LOOP_MAX; i++) {
		int mina, minb;
		double oldcost, mincost, cost;
		oldcost = EvaluatePath(size);
		mincost = INT_MAX;
		mina = -1; minb = -1;
		/* Evaluate M swaps (M is the number of nodes)  */
		for (j = 0; j < size; j++) {
			int a = rand() % size;
			int b = rand() % size;
			PathSwap(a, b);
			PrintPath(size);
			cost = EvaluatePath(size);
			if (cost < mincost) {
				if (cost < oldcost || !Queue_Find(a*size + b)) {
					mincost = cost;
					mina = a;
					minb = b;
				}
			}
			PathSwap(a, b);	/* Restore the path */
		}
		if (mina == -1 || minb == -1) continue;
		if (mincost >= oldcost) /* The best swap increases the cost */
			Queue_PushBack(mina*size + minb);/* Insert TabuList */
		PathSwap(mina, minb);
	}
	e = clock();
	printf("time = %d\n", e - s);
	printf("---------\n");
	PrintPath(size);
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
	if ((path = malloc(sizeof(int) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	InitialSolution(n);

	printf("[d]       Display TSP Problem\n");
	printf("[i]       Initialize Solution\n");
	printf("[t]       Tabu Search\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'i':	InitialSolution(n); break;
		case 't':	TabuSearch(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}
/*** Queue Functions ***/
double Queue[QUEUE_SIZE];
int Queue_Front;
int Queue_Back;
#define QUEUE_NEXT(i) (((i) + 1) % QUEUE_SIZE)
void Queue_Init() {
	Queue_Front = Queue_Back = 0;
}
int Queue_Find(double data) {
	int ptr;
	for (ptr = Queue_Front; ptr != Queue_Back; ptr = QUEUE_NEXT(ptr))
		if (data == Queue[ptr]) return 1;	/* Found */
	return 0;	/* Not found */
}
double Queue_PopFront() {
	double data;
	data = Queue[Queue_Front];
	Queue_Front = QUEUE_NEXT(Queue_Front);
	return data;
}
void Queue_PushBack(double data) {
	/* If the queue is full, then read out the front element */
	if (QUEUE_NEXT(Queue_Back) == Queue_Front) Queue_PopFront();
	Queue[Queue_Back] = data;
	Queue_Back = QUEUE_NEXT(Queue_Back);
}
/***********************/