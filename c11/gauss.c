#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#define BUF_SIZE 4096
double **matrix;
void Disp(int n) {
	int r, c;
	for (r = 0; r < n; r++) {
		for (c = 0; c < n + 1; c++)
			if (c == n) 	printf("| %2.2f", matrix[r][c]);
			else 		printf("%2.2f\t", matrix[r][c]);
		printf("\n");
	}
}
void Clear(int n) {
	int r;
	for (r = 0; r < n; r++) free(matrix[r]);
	free(matrix);
}
void Gauss(int n) {
	int p, r, c;
	double *x;
	if ((x = malloc(sizeof(double) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Forward elimination */
	for (p = 0; p < n - 1; p++) {
		double pivot = matrix[p][p];
		for (r = p + 1; r < n; r++) {
			double temp = matrix[r][p] / pivot;
			for (c = p + 1; c <= n; c++)
				matrix[r][c] -= matrix[p][c] * temp;
		}
	}
	/* Back substitution */
	for (p = n - 1; p >= 0; p--) {
		double temp = 0.0;
		for (c = p + 1; c < n; c++) temp += matrix[p][c] * x[c];
		x[p] = (matrix[p][n] - temp) / matrix[p][p];
	}
	/* Print out the result */
	for (p = 0; p < n; p++) printf("%2.2f ", x[p]);
	printf("\n");
	free(x);
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
	/* Read the file to get the number of variables (N) */
	fgets(buf, BUF_SIZE, fp);
	sscanf(buf, "%d", &n);

	/* Memory allocation for the input equations */
	if ((matrix = malloc(sizeof(double *) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < n; i++)
		if ((matrix[i] = malloc(sizeof(double) * (n + 1))) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	/* Initialize the matrix */
	for (i = 0; i < n; i++)
		for (j = 0; j < n + 1; j++)
			matrix[i][j] = 0;
	/* Read the matrix */
	i = 0;
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		char str[BUF_SIZE];
		char *p = buf;
		j = 0;
		while (sscanf(p, "%s", str) != EOF) {
			matrix[i][j++] = atof(str);
			p += strlen(str);
			while ((*p) && (isspace(*p))) p++;
		}
		i++;
	}
	fclose(fp);

	printf("[d]       Display Matrix\n");
	printf("[g]       Gaussian Elimination\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'g':	Gauss(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}