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
void GaussJordan(int n) {
	int p, r, c;
	for (p = 0; p < n; p++) {
		double pivot = matrix[p][p];
		for (c = 0; c < n + 1; c++) matrix[p][c] /= pivot;
		/* Elimination */
		for (r = 0; r < n; r++) {
			if (r != p) {
				double temp = matrix[r][p];
				for (c = p; c < n + 1; c++)
					matrix[r][c] -= matrix[p][c] * temp;
			}
		}
	}
	/* Print out the result */
	for (r = 0; r < n; r++) printf("%2.2f ", matrix[r][n]);
	printf("\n");
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int r, c, n;
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
	for (r = 0; r < n; r++)
		if ((matrix[r] = malloc(sizeof(double) * (n + 1))) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	/* Initialize the matrix */
	for (r = 0; r < n; r++)
		for (c = 0; c < n + 1; c++)
			matrix[r][c] = 0;
	/* Read the matrix */
	r = 0;
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		char str[BUF_SIZE];
		char *p = buf;
		c = 0;
		while (sscanf(p, "%s", str) != EOF) {
			matrix[r][c++] = atof(str);
			p += strlen(str);
			while ((*p) && (isspace(*p))) p++;
		}
		r++;
	}
	fclose(fp);

	printf("[d]       Display Matrix\n");
	printf("[j]       Gauss-Jordan Elimination\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'j':	GaussJordan(n); break;
		case 'e':	Clear(n); return 0;
		}
	}
	Clear(n);
	return 0;
}