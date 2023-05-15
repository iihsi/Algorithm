#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 4096
double *array_x, *array_y;
void Disp(int n) {
	int i;
	for (i = 0; i < n; i++)
		printf("x[%d] = %.3f\ty[%d] = %.3f\n",
				i, array_x[i], i, array_y[i]);
}
void Clear() {
	free(array_x);
	free(array_y);
}
void LeastSquares(int n) {
	int i;
	double A, B, C, D, E;
	double a, b;
	/* Initialization */
	A = 0.0; B = 0.0; C = 0.0; D = 0.0; E = 0.0;
	/* Calculate A, B, C, D, and E */
	for (i = 0; i < n; i++) {
		A += array_x[i] * array_x[i];
		B += array_x[i];
		C += array_x[i] * array_y[i];
		D += 1;
		E += array_y[i];
	}
	/* Calculate a and b for (y = a * x + b) */
	a = (C * D - B * E) / (A * D - B * B);
	b = (A * E - B * C) / (A * D - B * B);
	/* Print out the result */
	printf("y = %.3f * x + %.3f\n", a, b);
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
	/* Read the file to get the number of (x,y) pairs (N) */
	fgets(buf, BUF_SIZE, fp);
	sscanf(buf, "%d", &n);

	/* Memory allocation for the (x,y) pairs */
	if ((array_x = malloc(sizeof(double) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	if ((array_y = malloc(sizeof(double) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Read the (x,y) pairs */
	i = 0;
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		sscanf(buf, "%lf %lf", &array_x[i], &array_y[i]);
		i++;
	}
	fclose(fp);

	printf("[d]       Display (x,y) pairs\n");
	printf("[l]       Least-Squares Method\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'l':	LeastSquares(n); break;
		case 'e':	Clear(); return 0;
		}
	}
	Clear();
	return 0;
}