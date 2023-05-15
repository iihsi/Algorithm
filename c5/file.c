#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEXT_MAX    (1024 * 1024 * 10)	/* 10MB */
#define PATTERN_MAX (1024)
char	T[TEXT_MAX];
char	P[PATTERN_MAX];
void strstr_func(char *T, char *P) {
        char *ptr;
        if ((ptr = strstr(T, P)) != NULL)
                printf("Found at [%s]\n", ptr);
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
	printf("P[%d] = %s \n", (int)strlen(P), P);
	printf("T[%d] = %s \n", (int)strlen(T), T);

	printf("[s]       Perform strstr()\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 's':	strstr_func(T, P); break;
		case 'e':	return 0;
		}
	}
	return 0;
}