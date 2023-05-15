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
	int n, val;
	Init();
	printf("Input positive integer: ");
	scanf("%d", &n);
	while (n >= 1) {
		Push_Back(n); printf("Push %d\n", n);
		n--;
	}
	val = 1;
	while (!Empty()) {
		n = Pop_Back(); printf("Pop %d\n", n);
		val *= n;
	}
	printf("fact(%d) = %d\n", n, val);
	return 0;
}