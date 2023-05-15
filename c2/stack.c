#include <stdio.h>
#define STACK_SIZE 256
int stack[STACK_SIZE];
int ptr;
void Init() {
	ptr = 0;
}
int Empty() {
	return (ptr == 0);
}
int Size() {
	return ptr;
}
void Push_Back(int data) {
	if (ptr < STACK_SIZE)
		stack[ptr++] = data;
}
int Pop_Back() {
	if (Empty())
		return -1;
	return stack[--ptr];
}
int main() {
	int data; char command;
	Init();
	printf("[e]       Empty or Not\n");
	printf("[s]       Size\n");
	printf("[u <num>] Push Back\n");
	printf("[o]       Pop Back\n");
	printf("[b]       Bye\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'e':	printf("Empty %d\n", Empty()); break;
		case 's':	printf("Size %d\n", Size()); break;
		case 'u':	scanf("%d", &data); Push_Back(data); break;
		case 'o':	printf("%d\n", Pop_Back()); break;
		case 'b':	return 0;
		}
	}
	return 0;
}