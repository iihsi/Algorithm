#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUF_SIZE 80
#define NAME_SIZE 32
struct student {
	int score;
	char name[NAME_SIZE];
};
struct student *table;
int counter_compare, counter_swap;
void Clear() {
	counter_compare = 0; counter_swap = 0;
}
void Counter() {
	printf("Compare %d Swap %d\n", counter_compare, counter_swap);
}
int my_compare(const struct student *a, const struct student *b) {
	counter_compare++;
	return (a->score > b->score) ? 1 : (a->score < b->score) ? -1 : 0;
}
void my_swap(struct student *a, struct student *b) {
	struct student c;
	counter_swap++;
	memcpy(&c, a, sizeof(struct student));
	memcpy(a, b, sizeof(struct student));
	memcpy(b, &c, sizeof(struct student));
}
void Bubble(int size) {
	int i, j;
	for (i = 0; i < size - 1; i++)
		for (j = size - 1; j > i; j--)
			/* If table[j-1] is larger than table[j] */
			if (my_compare(&table[j - 1], &table[j]) == 1)
				/* Swap table[j-1] and table[j] */
				my_swap(&table[j-1], &table[j]);
}
void Select(int size) {
	int i, j;
	for (i = 0; i < size - 1; i++) {
		int min = i;
		for (j = i + 1; j < size; j++)
			/* If table[min] is larger than table[j] */
			if (my_compare(&table[min], &table[j]) == 1)
				/* Update min with j */
				min = j;
		/* Swap table[i] and table[min] */
		my_swap(&table[i], &table[min]);
	}
}
void Insert(int size) {
	int i, j;
	for (i = 1; i < size; i++) {
		j = i;
		/* While table[j - 1] is larger than table[j] */
		while (j >= 1 && my_compare(&table[j - 1], &table[j]) == 1/* table[j - 1] is larger than table[j] */) {
			/* Swap table[j] and table[j-1] */
			my_swap(&table[j], &table[j - 1]);
			j--;
		}
	}
}
void Shell(int size) {
	int i, j, gap;
	gap = size / 2;
	while (gap > 0) {
		for (i = gap; i < size; i++) {
			/* Begin Insertion Sort */
			j = i;
			while (j >= gap &&
				my_compare(&table[j-gap], &table[j]) == 1) {
				my_swap(&table[j], &table[j-gap]);
				j -= gap;
			}
			/* End Insertion Sort */
		}
		gap = gap / 2;
	}
}
void Disp(int size) {
	int i;
	for (i = 0; i < size; i++)
		printf("%d\t%s\n", table[i].score, table[i].name);
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int n;
	char buf[BUF_SIZE];
	char command;
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]); exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file (%s) \n", argv[1]); exit(1);
	}
	/* Read the file to count the number of lines in the file */
	n = 0;
	while (fgets(buf, BUF_SIZE, fp) != NULL)
		n++;
	/* Memory allocation */
	if ((table = malloc(sizeof(struct student) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Read the file again to copy the data */
	n = 0;
	fseek(fp, 0L, SEEK_SET);
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		sscanf(buf, "%d\t%s", &table[n].score, table[n].name);
		n++;
	}
	fclose(fp);

	Clear();
	printf("[d]       Display Table\n");
	printf("[h]       Shell Sort\n");
	printf("[i]       Insertion Sort\n");
	printf("[s]       Selection Sort\n");
	printf("[b]       Bubble Sort\n");
	printf("[c]       Clear Counters\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(n); break;
		case 'h':	Shell(n); Counter(); break;
		case 'i':	Insert(n); Counter(); break;
		case 's':	Select(n); Counter(); break;
		case 'b':	Bubble(n); Counter(); break;
		case 'c':	Clear(); break;
		case 'e':	free(table); return 0;
		}
	}
	return 0;
}