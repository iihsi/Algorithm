#include <stdio.h>
#define QUEUE_SIZE 256
int queue[QUEUE_SIZE];
int ptr_front;
int ptr_back;
#define QUEUE_NEXT(i) (((i) + 1) % QUEUE_SIZE)
void Init() {
	ptr_front = ptr_back = 0;
}
int Empty() {
	return (ptr_front == ptr_back);
}
int Size() {
	return (ptr_back >= ptr_front) ? (ptr_back - ptr_front) : 
			(QUEUE_SIZE - (ptr_front - ptr_back));
}
void Push_Back(int data) {
	if (QUEUE_NEXT(ptr_back) != ptr_front) {
		queue[ptr_back] = data;
		ptr_back = QUEUE_NEXT(ptr_back);
	}
}
int Pop_Front() {
	int data;
	if (Empty())
		return -1;
	data = queue[ptr_front];
	ptr_front = QUEUE_NEXT(ptr_front);
	return data;
}
int main() {
	int data; char command;
	Init();
	printf("[e]       Empty or Not\n");
	printf("[s]       Size\n");
	printf("[u <num>] Push Back\n");
	printf("[o]       Pop Front\n");
	printf("[b]       Bye\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'e':	printf("Empty %d\n", Empty()); break;
		case 's':	printf("Size %d\n", Size()); break;
		case 'u':	scanf("%d", &data); Push_Back(data); break;
		case 'o':	printf("%d\n", Pop_Front()); break;
		case 'b':	return 0;
		}
	}
	return 0;
}