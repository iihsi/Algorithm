#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#define TEXT_MAX    (1024 * 1024 * 10)  /* 10MB */
#define PATTERN_MAX (1024)
char    T[TEXT_MAX];
char    P[PATTERN_MAX];
int counter_compare;
void Clear() {
	counter_compare = 0;
}
void Counter() {
	printf("Compare %d\n", counter_compare);
}
int my_compare(const char a, const char b) {
	counter_compare++;
	return (a == b) ? 1 : 0;
}
int BruteForce(char *T, int n, char *P, int m) {
	struct timeval stime, etime;
	double sec;
	gettimeofday(&stime, NULL);

	int i, p;
	/* For all characters in T[0] ... T[n-m] */
	for (i = 0; i <= n - m; i++) {
		/* For all characters in P[0] ... P[m-1] */
		for (p = 0; p < m; p++)
			/* If T[i+p] != P[p] */
			if (my_compare(T[i + p], P[p]) == 0)
				break;
		if (p == m) return i;
	}
	return -1;

	gettimeofday($etime, NULL);
	sec = (etime.tv_sec - stime.tv_sec) + (etime.tv_usec - stime.tv_usec)/1000000.0;
	printf("Elapsed time %.6f [sec]\n", sec);
}
void BruteForce_func(char *T, char *P) {
	int ptr;
	if ((ptr = BruteForce(T, strlen(T), P, strlen(P))) != -1)
		printf("Found at T[%d]\n", ptr);
	else
		printf("Not found \n");
}
int KMP(char *T, int n, char *P, int m) {
	int i, p;
	int table[PATTERN_MAX];
	/* Make a skip table */
	for (p = -1, i = 0; i < m; i++) {
		table[i] = p;
		while ((p >= 0) && (P[i] != P[p]))
			p = table[p];
		p++;
	}
	/* Print the skip table */
	for (i = 0; i < m; i++)
		printf("%d ", table[i]);
	printf("\n");
	/* String match */
	for (p = 0, i = 0; i < n; i++) {
		while ((p >= 0) && (my_compare(T[i], P[p]) == 0))
			p = table[p];
		if (p == m - 1)
			return i - m + 1;
		p++;
	}
	return -1;
}
void KMP_func(char *T, char *P) {
	int ptr;
	if ((ptr = KMP(T, strlen(T), P, strlen(P))) != -1)
		printf("Found at T[%d]\n", ptr);
	else
		printf("Not found \n");
}
int BM(char *T, int n, char *P, int m) {
	int i, p;
	int table[UCHAR_MAX + 1];
	/* Make a skip table */
	for (i = 0; i <= UCHAR_MAX; i++)
        table[i] = m;
    for (i = 0; i < m - 1; i++)
        table[ (int)P[i] ] = m - i - 1;

	/* Print the skip table */
	for (i = '0'; i < 'z'; i++)
		if (table[i] != m)
			printf("[%c]=%d ", i, table[i]);
	printf("\n");
	/* String match */
	i = m - 1;
	while (i < n) {
		p = m - 1;
		while (my_compare(T[i], P[p])) {
			if (p == 0) return i;
			p--; i--;
		}
		i += (table[ (int)T[i] ] > m - p) ?
			table[ (int) T[i] ] : m - p;
	}
	return -1;
}
void BM_func(char *T, char *P) {
	int ptr;
	if ((ptr = BM(T, strlen(T), P, strlen(P))) != -1)
		printf("Found at T[%d]\n", ptr);
	else
		printf("Not found \n");
}


int main(int argc, char *argv[]) {
	FILE *fp;
	char command;
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]); exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file (%s) \n", argv[1]); exit(1);
	}
	/* Read the file to get the pattern P */
	fgets(P, PATTERN_MAX, fp);
	P[strlen(P) - 1] = 0;
	/* Read the file to get the text T */
	fgets(T, TEXT_MAX, fp);
	T[strlen(T) - 1] = 0;
	fclose(fp);
	//printf("P[%d] = %s \n", strlen(P), P);
	//printf("T[%d] = %s \n", strlen(T), T);

	Clear();
	printf("[b]       Brute-Force Algorithm\n");
	printf("[k]       Knuth-Morris-Pratt Algorithm\n");
	printf("[m]       Boyer-Moore Algorithm\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'b':	BruteForce_func(T, P); Counter(); break;
		case 'k':	KMP_func(T, P); Counter(); break;
		case 'm':	BM_func(T, P); Counter(); break;
		case 'e':	return 0;
		}
	}
	return 0;
}