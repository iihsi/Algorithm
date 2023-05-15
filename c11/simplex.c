#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#define BUF_SIZE 4096
double **matrix;
void Disp(int nr, int nc) {
	int r, c;
	for (r = 0; r < nr; r++) {
		for (c = 0; c < nc; c++)
			if (c == nc - 1)printf("| %2.2f", matrix[r][c]);
			else 		printf("%2.2f\t", matrix[r][c]);
		printf("\n");
	}
}
void Clear(int nr, int nc) {
	int r;
	for (r = 0; r < nr; r++) free(matrix[r]);
	free(matrix);
}
int Select_MinCol(int nr, int nc) {
	int c, minc; double min = INT_MAX;
	for (c = 0; c < nc - 1; c++)
		if (matrix[nr - 1][c] < min) {
			min = matrix[nr - 1][c];
			minc = c;
		}
	/* If min >=0, there is no better solution (return -1). */ 
	return (min < 0) ? minc : -1;
}
int Select_MinRow(int nr, int nc, int minc) {
	int r, minr; double min = INT_MAX;
	for (r = 0; r < nr - 1; r++) {
		double temp = matrix[r][nc - 1] / matrix[r][minc];
		if (matrix[r][minc] > 0 && temp < min) {
			min = temp;
			minr = r;
		}
	}
	return minr;
}
void Simplex(int nr, int nc) {
	int minc, minr; double pivot;
	while (1) {
		int c, r;
		minc = Select_MinCol(nr, nc);
		if (minc == -1) break;
		minr = Select_MinRow(nr, nc, minc);
		pivot = matrix[minr][minc];
		printf("Col=%d Row=%d Pivot=%2.2f\n", minc, minr, pivot);

		/* Elimination (same as Gauss-Jordan method) */

		/* Use the inner two loops of Gauss-Jordan method */  
		/* Note that pivot is matrix[minr][minc] */
        for (c = 0; c < nc; c++)
            matrix[minr][c] /= pivot;
        for (r = 0; r < nr; r++)
            if (r != minr) {
                double temp  = matrix[r][minc];
                for (c = 0; c < nc; c++)
                    matrix[r][c] -= matrix[minr][c] * temp;
            }
		Disp(nr, nc);
	}
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int r, c, nr, nc, nv;
	char buf[BUF_SIZE];
	char command;
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]); exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file (%s) \n", argv[1]); exit(1);
	}
	/* Read the file to get the problem size */
	/* NR: number of rows; NC: number of columns; NV: number of variables */
	fgets(buf, BUF_SIZE, fp);
	sscanf(buf, "%d %d %d", &nr, &nc, &nv);

	/* Memory allocation for the input equations */
	if ((matrix = malloc(sizeof(double *) * nr)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (r = 0; r < nr; r++)
		if ((matrix[r] = malloc(sizeof(double) * nc)) == NULL) {
			printf("Cannot allocate memory \n"); exit(1);
		}
	/* Initialize the matrix */
	for (r = 0; r < nr; r++)
		for (c = 0; c < nc; c++)
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
	printf("[s]       Linear programming (Simplex)\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(nr, nc); break;
		case 's':	Simplex(nr, nc); break;
		case 'e':	Clear(nr, nc); return 0;
		}
	}
	Clear(nr, nc);
	return 0;
}