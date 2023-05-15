#include <stdio.h>
#include <stdlib.h>
struct node {
	int data;
	struct node *next;
	struct node *prev;
};
struct node head;
void Init() {
	head.next = head.prev = &head;
}
int Empty() {
	return (head.next == &head);
}
void Push(int data) {
	struct node *p;
	if ((p = malloc(sizeof(struct node))) == NULL) {
		printf("Memory allocation error\n"); exit(1);
	}
	p->data = data;
	p->prev = &head;
	p->next = head.next;
	head.next->prev = p;
	head.next = p;
}
int Pop() {
	struct node *p;
	int data;
	if (Empty())
		return -1;
	p = head.next;
	data = p->data;
	p->prev->next = p->next;
	p->next->prev = p->prev;
	free(p);
	return data;
}
void Clear() {
	while (!Empty())
		Pop();
}
void Disp() {
	struct node *p;
	p = head.next;
	while (p != &head) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}
void Disp_Reverse() {
    struct node *p;
    p = head.prev;
    while (p != &head) {
        printf("%d ", p->data);
        p = p->prev;
    }
    printf("\n");
}
int Find(int key) {
	struct node *p;
	p = head.next;
	while (p != &head) {
		if (p->data == key)
			return 1;
		p = p->next;
	}
	return 0;
}
void Insert_After(int data, int key) {
	struct node *n, *p;
	if ((n = malloc(sizeof(struct node))) == NULL) {
		printf("Memory allocation error\n"); exit(1);
	}
	p = head.next;
	while (p != &head) {
		if (p->data == key) {
			n->data = data;
			n->prev = p;
			n->next = p->next;
			p->next->prev = n;
			p->next = n;
			return;
		}
		p = p->next;
	}
}
void Insert_Before(int data, int key) {
}
void Remove(int key) {
	struct node *p;
	p = head.next;
	while (p != &head) {
		if (p->data == key) {
			p->prev->next = p->next;
			p->next->prev = p->prev;
			free(p);
			return;
		}
		p = p->next;
	}
}
int main() {
	int data; int key; char command;
	Init();
	printf("[e]       Empty or Not\n");
	printf("[d]       Display List\n");
	printf("[u <num>] Push\n");
	printf("[o]       Pop\n");
	printf("[f <num>] Find Value\n");
	printf("[s <num1> <num2>] Insert <num1> After <num2>\n");
	printf("[t <num1> <num2>] Insert <num1> Before <num2>\n");
	printf("[r <num>] Remove Value\n");
	printf("[b]       Bye\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'e':	printf("Empty %d\n", Empty()); break;
		case 'd':	Disp(); break;
        case 'v':   Disp_Reverse(); break;
		case 'u':	scanf("%d", &data); Push(data); break;
		case 'o':	printf("%d\n", Pop()); break;
		case 'f':	scanf("%d", &data); printf("Find %d\n", Find(data)); break;
		case 's':	scanf("%d %d", &data, &key); Insert_After(data, key); break;
		case 't':	scanf("%d %d", &data, &key); Insert_Before(data, key); break;
		case 'r':	scanf("%d", &data); Remove(data); break;
		case 'b':	Clear(); return 0;
		}
	}
	return 0;
}