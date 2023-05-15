#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 80
#define NAME_SIZE 32
#define HASH_SIZE 1024
struct student {
	int score;
	char name[NAME_SIZE];
	struct student *next;
};
int counter_compare, counter_copy = 0;
struct student *data, **table;
int my_compare(const struct student *a, const struct student *b) {
	counter_compare++;
	return strcmp(a->name, b->name);
}
void my_copy(struct student *src, struct student *dst) {
	counter_copy++;
	memcpy(dst, src, sizeof(struct student));
}
int my_hash(struct student *d) {
	int i, n = 0;
	for (i = 0; i < strlen(d->name); i++) n += d->name[i];
        return n % HASH_SIZE;
}
void Insert(struct student *d) {
	struct student *p;
	int h = my_hash(d);
	if ((p = malloc(sizeof(struct student))) == NULL) {
		printf("Memory allocation error\n"); exit(1);
	}
	/* Copy d to p */
	my_copy(d, p);
	/* Insert p to the list */
	p->next = table[h]; table[h] = p;
}
int Search(struct student *d) {
	struct student *p;
	int h = my_hash(d);
	/* If the node exists on the list, then return 1 */
	for (p = table[h]; p != NULL; p = p->next)
        if (my_compare(p, d) == 0) return 1;
	/* Otherwise return 0 */
	return 0;
}
void Remove(struct student *d) {
	struct student *p, *q;
	int h = my_hash(d);
	if (table[h] == NULL) return;
	/* Key node is on the top of the chain */ 
	/* If table[h] is equal to d */
	if (my_compare(table[h], d) == 0) {
		/* Remove the node from the list */
		p = table[h]; table[h] = p->next; free(p);
		return;
	}
	/* Otherwise */
	for (q = table[h], p = q->next; p != NULL; q = p, p = p->next)
		/* If p is equal to d */
		if (my_compare(p, d) == 0) {
			/* Remove the node from the list */
			q->next = p->next; free(p);
			return;
		}
}
void Disp() {
	int i;
	struct student *p;
	for (i = 0; i < HASH_SIZE; i++) {
		if (table[i] == NULL) continue;
		printf("Hash %d : ", i);
		for (p = table[i]; p != NULL; p = p->next)
			printf("(%d %s) ", p->score, p->name);
		printf("\n");
	}
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
	int i, n;
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
	if ((table = malloc(sizeof(struct student *) * HASH_SIZE)) == NULL) {
		printf("Cannot allocate memory \n"); exit(1);
	}
	for (i = 0; i < HASH_SIZE; i++) table[i] = NULL;

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