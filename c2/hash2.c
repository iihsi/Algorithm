#include <stdio.h>
#define ARRAY_SIZE 8
struct node {
	int data;
	int empty;
	int deleted;
};
struct node table[ARRAY_SIZE];
#define HASH(i) ((i) % ARRAY_SIZE)
#define REHASH(i) (((i) + 1) % ARRAY_SIZE)
void Init() {
	int i;
	for (i = 0; i < ARRAY_SIZE; i++) {
		table[i].empty = 1;
		table[i].deleted = 0;
	}
}
void Insert(int data) {
	int n = HASH(data);
	while (!table[n].empty && !table[n].deleted) {
		if (table[n].data == data) return;
		n = REHASH(n);
	}
	table[n].data = data;
	table[n].empty = 0;
	table[n].deleted = 0;
}
void Remove(int data) {
	int n = HASH(data);
	while (!table[n].empty || table[n].deleted) {
		if (!table[n].deleted && table[n].data == data) {
			table[n].data = 0;
			table[n].empty = 1;
			table[n].deleted = 1;
			return;
		}
		n = REHASH(n);
	}
}
void Disp() {
	int i;
	for (i = 0; i < ARRAY_SIZE; i++)
		if (table[i].empty)
			printf("[%d]=  ", i);
		else
			printf("[%d]=%d ", i, table[i].data);
	printf("\n");
}
int main() {
	int data; char command;
	Init();
	printf("[s <num>] Insert Value\n");
	printf("[r <num>] Remove Value\n");
	printf("[d]       Display Table\n");
	printf("[b]       Bye\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 's':	scanf("%d", &data); Insert(data); break;
		case 'r':	scanf("%d", &data); Remove(data); break;
		case 'd':	Disp(); break;
		case 'b':	return 0;
		}
	}
	return 0;
}