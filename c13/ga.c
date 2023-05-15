#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#define BUF_SIZE 80
#define GROUP_SIZE 16	/* Num of individuals in generation (tuning needed) */
#define LOOP_MAX 2000	/* Num of iterations of the main loop (tuning needed) */
/*
   Probability for Mutation : MUTATION_RATE
   Probability for Crossover: CROSSOVER_RATE
   Probability for Copy     : 1.00 - MUTATION_RATE - CROSSOVER_RATE
*/
#define MUTATION_RATE	0.05	/* tuning needed */
#define CROSSOVER_RATE	0.50	/* tuning needed */
struct node {
	double NS;	/* latitude */
	double EW;	/* longitude */
};
struct node *nodes;
double **dist;
int **oldgroup, **newgroup;	/* old and new generations */
void PathSwap(int *path, int a, int b) {
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
	free(dist); free(nodes);
}
double EvaluatePath(int *path, int size) {
	int i;
	double cost = 0;
	for (i = 1; i < size; i++) cost += dist[path[i - 1]][path[i]];
	cost += dist[path[size - 1]][path[0]];
	return cost;
}
void PrintGeneration(int **ppath, int size) {
	int i, n;
	for (n = 0; n < GROUP_SIZE; n++) {
		for (i = 0; i < size; i++) printf("%2d ", ppath[n][i]);
		printf("%2d : Cost = %f\n", ppath[n][0],
				EvaluatePath(ppath[n], size));
	}
}
int Selection(int size) {
	int i;
	double mincost, cost;
	double *score;
	double rnd, val = 0.0, totalscore = 0.0;
	if ((score = malloc(sizeof(double) * GROUP_SIZE)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Find the minimum cost in oldgroup */
	mincost = INT_MAX;
	for (i = 0; i < GROUP_SIZE; i++) {
		cost = EvaluatePath(oldgroup[i], size);
		if (mincost > cost) mincost = cost;
	}
	/* Score is defined as (1 / (pathcost - mincost)); Larger is better */
	for (i = 0; i < GROUP_SIZE; i++) {
		cost = EvaluatePath(oldgroup[i], size);
		score[i] = 1 / (cost - mincost);
		totalscore += score[i];
	}
	/* Random value (0.0 - totalscore) */
	rnd = ((double)rand() / RAND_MAX) * totalscore;
	for (i = 0; i < GROUP_SIZE; i++) {
		if (val <= rnd && rnd <= val + score[i]) {
			free(score); return i;
		}
		val += score[i];
	}
	free(score); return GROUP_SIZE - 1;
}
void RandomSolution(int *path, int size) {
	int i;
	for (i = 0; i < size; i++) path[i] = i;
	/* Swap two nodes randomly */
	for (i = 0; i < size; i++) PathSwap(path, rand() % size, rand() % size);
}
void Copy(int *path, int size) {
	int a;
	a = Selection(size);	/* Select an individual from oldgroup */
	memcpy(path, oldgroup[a], sizeof(int) * size);
}
void Mutation(int *path, int size) {
	int a;
	a = Selection(size);	/* Select an individual from oldgroup */
	/* Copy from oldgroup[a] to path */
	memcpy(path, oldgroup[a], sizeof(int) * size);
	/* Swap two nodes randomly */
	PathSwap(path, rand() % size, rand() % size);
}
void Crossover(int *path, int size) {
	int i, j, a, b;
	int m, n, begin, end;
	a = Selection(size);		/* Select an individual from oldgroup */
	b = Selection(size);		/* Select an individual from oldgroup */
	while (1) {
		int legal = 1;
		/* Select two crossover points (begin & end) randomly */
		m = rand() % size;
		n = rand() % size;
		if (m > n) { begin = n; end = m; }
		else       { begin = m; end = n; }
		memcpy(path, oldgroup[a], sizeof(int) * size);
		for (i = begin; i <= end; i++) path[i] = oldgroup[b][i];
		/* If this path is not Hamiltonian path, continue the loop */
		for (i = 0; i < size; i++) {
			int found = 0;
			for (j = 0; j < size; j++) {
				if (path[j] == i) found++;
				if (found > 1) { legal = 0; break; }
			}
			if (legal == 0) break;
		}
		if (legal == 1) return;
	}
}
void GeneticAlgorithm(int size) {
	long s, e;
	s = clock();
	int n, i;
	/* Step 1: Memory allocation */
	if ((oldgroup = malloc(sizeof(int*) * GROUP_SIZE)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((newgroup = malloc(sizeof(int*) * GROUP_SIZE)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < GROUP_SIZE; i++) {
		if ((oldgroup[i] = malloc(sizeof(int) * size)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
		if ((newgroup[i] = malloc(sizeof(int) * size)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	}

	/* Step 2: Initial solution */
	srand((unsigned int)time(NULL));
	for (i = 0; i < GROUP_SIZE; i++) RandomSolution(oldgroup[i], size);

	for (n = 0; n < LOOP_MAX; n++) {
		/* Step 3: Making new generation based on old generation */
		for (i = 0; i < GROUP_SIZE; i++) {
			/* random value (0.0 - 1.0) */
			double rnd = (double)rand() / RAND_MAX;
			if (rnd < MUTATION_RATE)
				Mutation(newgroup[i], size);
			else if (rnd < CROSSOVER_RATE + MUTATION_RATE)
				Crossover(newgroup[i], size);
			else
				Copy(newgroup[i], size);
		}
		printf("********** Generation %d **********\n", n);
		printf("----- Old group -----\n");
		PrintGeneration(oldgroup, size);
		printf("----- New group -----\n");
		PrintGeneration(newgroup, size);

		/* Step 4: New generation becomes old */
		for (i = 0; i < GROUP_SIZE; i++)
			memcpy(oldgroup[i], newgroup[i], sizeof(int) * size);
	}

	/* Step 5: Find the best solution */

	/* Step 6: Memory free */
	for (i = 0; i < GROUP_SIZE; i++) {
		free(oldgroup[i]); free(newgroup[i]);
	}
	free(oldgroup); free(newgroup);
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

	printf("[d]       Display TSP Problem\n");
	printf("[g]       Genetic Algorithm\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'g':	GeneticAlgorithm(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}