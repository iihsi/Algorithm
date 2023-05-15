#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 80
#define NAME_SIZE 32
struct student {
	int score;
	char name[NAME_SIZE];
};
int ptr, counter_compare, counter_copy = 0;
struct student *data, *table;
int my_compare(const struct student *a, const struct student *b) {
	counter_compare++;
	return strcmp(a->name, b->name);
}
void my_copy(struct student *src, struct student *dst) {
	counter_copy++;
	memcpy(dst, src, sizeof(struct student));
}
void Insert(struct student *d) {
	my_copy(d, &table[ptr]); ptr++;
}
int Search(struct student *d) {
	int i;
	for (i = 0; i < ptr; i++)
		/* If table[i] is equal to d, return 1 */
		if (my_compare(&table[i], d) == 0) return 1;
	return 0;
}
void Remove(struct student *d) {
	int i, j;
	for (i = 0; i < ptr; i++)
		/* If table[i] is equal to d */
		if (my_compare(&table[i], d) == 0) {
			for (j = i; j < ptr - 1; j++)
				/* Copy table[j+1] to table[j] */
				my_copy(&table[j + 1], &table[j]);
			ptr--;
			return;
		}
}
void Disp() {
	int i;
	for (i = 0; i < ptr; i++)
		printf("%d\t%s\n", table[i].score, table[i].name);
}
void All_Insert(int size) {
	int i;
	counter_compare = 0; counter_copy = 0;
	for (i = 0; i < size; i++) Insert(&data[i]);
	printf("Compare %d Copy %d\n", counter_compare/size, counter_copy/size);
}
void All_Search(int size) {
	int i;
	counter_compare = 0; counter_copy = 0;
	for (i = 0; i < size; i++) Search(&data[i]);
	printf("Compare %d Copy %d\n", counter_compare/size, counter_copy/size);
}
void All_Remove(int size) {
	int i;
	counter_compare = 0; counter_copy = 0;
	for (i = 0; i < size; i++) Remove(&data[i]);
	printf("Compare %d Copy %d\n", counter_compare/size, counter_copy/size);
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
	if ((data = malloc(sizeof(struct student) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	/* Read the file again to copy the data */
	n = 0;
	fseek(fp, 0L, SEEK_SET);
	while (fgets(buf, BUF_SIZE, fp) != NULL) {
		sscanf(buf, "%d\t%s", &data[n].score, data[n].name);
		n++;
	}
	fclose(fp);

	/* Memory allocation */
	if ((table = malloc(sizeof(struct student) * n)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}

	printf("[d]       Display Table\n");
	printf("[i]       All Insert\n");
	printf("[s]       All Search\n");
	printf("[r]       All Remove\n");
	printf("[e]       Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'd':	Disp(); break;
		case 'i':	All_Insert(n); break;
		case 's':	All_Search(n); break; 
		case 'r':	All_Remove(n); break;
		case 'e':	free(data); free(table); return 0;
		}
	}
	return 0;
}