#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#define BUF_SIZE 80
#define COOL_RATE 0.9999	/* Cooling rate (tuning needed) */
struct node {
	double NS;	/* latitude */
	double EW;	/* longitude */
};
struct node *nodes;
double **dist;
int *path;
int Probability(double cost, double newcost, double temp) {
	double rnd = (double)rand() / RAND_MAX;  /* random value (0.0 - 1.0) */
	double prob = exp((cost - newcost) / temp);
	return (prob > rnd) ? 1 : 0;
}
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
void SimulatedAnnealing(int size) {
	long s, e;
	s = clock();
	double temp;
	for (temp = 1000.0; temp > 0.1; temp *= COOL_RATE) {
		int a, b;
		double cost, newcost;
		/* Evaluate the current cost */
		cost = EvaluatePath(size);
		/* Two nodes are randomly selected */
		a = rand() % size ; b = rand() % size ;
		/* The two nodes are swapped */
		PathSwap(a, b);
		/* Evaluate updated cost */
		newcost = EvaluatePath(size);
		/* Taken or not-taken based on the probability */
		if (Probability(cost, newcost, temp))
			PrintPath(size);    /* Taken (Use this path) */
		else
			PathSwap(a, b) ;               /* Not-taken (Restore the path) */
	}
	e = clock();
	printf("time = %d\n", e - s);
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
	printf("[s]       Simulated Annealing Search\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'i':	InitialSolution(n); break;
		case 's':	SimulatedAnnealing(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}