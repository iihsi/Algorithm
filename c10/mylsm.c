#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#define BUF_SIZE 4096
double *array_x, *array_y;

void Disp(int n) {
        int i;
	for (i = 0; i < n; i++)
		printf("x[%d] = %.3f\ty[%d] = %.3f\n", i, array_x[i], i, array_y[i]);
}

void Clear() {
	free(array_x);
	free(array_y);
}

void LeastSquares(int n) {
	int i;
	double A, B, C, D, E, F, G, H;
        int p, r, c;
	double x[3];
        double matrix[3][4];
	A = 0.0; B = 0.0; C = 0.0; D = 0.0; E = 0.0; F = 0.0; G = 0.0; H = 0.0;
	for (i = 0; i < n; i++) {
		A += array_x[i] * array_x[i] * array_x[i] * array_x[i];
		B += array_x[i] * array_x[i] * array_x[i];
		C += array_x[i] * array_x[i];
		D += array_x[i];
		E += 1;
                F += array_x[i] * array_x[i] * array_y[i];
                G += array_x[i] * array_y[i];
                H += array_y[i];
	}
        matrix[0][0] = A;
        matrix[0][1] = B;
        matrix[0][2] = C;
        matrix[0][3] = F;
        matrix[1][0] = B;
        matrix[1][1] = C;
        matrix[1][2] = D;
        matrix[1][3] = G;
        matrix[2][0] = C;
        matrix[2][1] = D;
        matrix[2][2] = E;
        matrix[2][3] = H;
	for (p = 0; p < 2; p++) {
		double pivot = matrix[p][p];
		for (r = p + 1; r < 3; r++) {
			double temp = matrix[r][p] / pivot;
			for (c = p + 1; c <= 3; c++)
				matrix[r][c] -= matrix[p][c] * temp;
		}
	}
	for (p = 2; p >= 0; p--) {
		double num = 0.0;
		for (c = p + 1; c < 3; c++) num += matrix[p][c] * x[c];
		x[p] = (matrix[p][3] - num) / matrix[p][p];
	}
	printf("y = %.3f * x^2 + %.3f * x + %.3f\n", x[0], x[1], x[2]);
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