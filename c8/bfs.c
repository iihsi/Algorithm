#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEPTH 9 
int counter_compare = 0;
int swap_with_upper(int *board, int *next);
int swap_with_lower(int *board, int *next);
int swap_with_right(int *board, int *next);
int swap_with_left(int *board, int *next);
long board2val(int *board);
void val2board(long val, int *board);
/*** Queue/Hash functions ***/
#define QUEUE_SIZE 1024
#define HASH_SIZE 1024
void Queue_Init();
int Queue_Size();
void Queue_Push(long val);
long Queue_Pop();
void Hash_Init();
void Hash_Insert(long val);
int Hash_Search(long val);
void Hash_Clear();
/*****************************/
int my_compare(int *board1, int *board2) {
	counter_compare++;
	return memcmp(board1, board2, sizeof(int) * 9);
}
void print_board(int *board, int depth) {
	printf("Depth: %d\n", depth);
	printf("%2d%2d%2d\n", board[0], board[1], board[2]);
	printf("%2d%2d%2d\n", board[3], board[4], board[5]);
	printf("%2d%2d%2d\n", board[6], board[7], board[8]);
}
/* Push board state to Queue */
void bfs_push(int *board, int depth) {
	Queue_Push(board2val(board));
	Queue_Push((int)depth);
}
/* Pop board state from Queue */
void bfs_pop(int *board, int *depth) {
	long val = Queue_Pop();
	*depth = (int)Queue_Pop();
	val2board(val, board);
}
int bfs_empty() {
	return Queue_Size() == 0;
}
/* Set board state as "visited" */
void set_visited(int *board) {
	Hash_Insert(board2val(board));
}
/* Is board state "visited"? */
int is_visited(int *board) {
	return Hash_Search(board2val(board));
}
void BFS(int *init, int *goal) {
	int depth = 0;
	bfs_push(init, depth);
	print_board(init, depth);
	if (!my_compare(init, goal)) {
		printf("Found!\n"); return;
	}
	while (!bfs_empty()) {
		int curr[9], next[9];
		bfs_pop(curr, &depth);
		set_visited(curr);
		depth++;
		/* Swap with upper */
		if (swap_with_upper(curr, next) != -1) {
			if (!is_visited(next)) {
				print_board(next, depth);
				if (!my_compare(next, goal)) {
					printf("Found!\n"); return;
				}
				if (depth < MAX_DEPTH) bfs_push(next, depth);
			}
		}
		/* Swap with lower */
		if (swap_with_lower(curr, next) != -1) {
			if (!is_visited(next)) {
				print_board(next, depth);
				if (!my_compare(next, goal)) {
					printf("Found!\n"); return;
				}
				if (depth < MAX_DEPTH) bfs_push(next, depth);
			}
		}
		/* Swap with right */
		if (swap_with_right(curr, next) != -1) {
			if (!is_visited(next)) {
				print_board(next, depth);
				if (!my_compare(next, goal)) {
					printf("Found!\n"); return;
				}
				if (depth < MAX_DEPTH) bfs_push(next, depth);
			}
		}
		/* Swap with left */
		if (swap_with_left(curr, next) != -1) {
			if (!is_visited(next)) {
				print_board(next, depth);
				if (!my_compare(next, goal)) {
					printf("Found!\n"); return;
				}
				if (depth < MAX_DEPTH) bfs_push(next, depth);
			}
		}
	}
	printf("Not Found...\n");
}
int main(int argc, char *argv[]) {
	FILE *fp;
	int init[9];
	int goal[9] = {1, 2, 3, 8, 0, 4, 7, 6, 5};	/* Target board */
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]); exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file (%s) \n", argv[1]); exit(1);
	}
	fscanf(fp, "%d %d %d %d %d %d %d %d %d",
		&init[0], &init[1], &init[2], &init[3],
		&init[4], &init[5], &init[6], &init[7], &init[8]);
	fclose(fp);

	counter_compare = 0;
	Queue_Init();
	Hash_Init();
	BFS(init, goal);
	printf("Compare %d\n", counter_compare);
	Hash_Clear();
	return 0;
}
/* Board state swapping functions */
int swap_with_upper(int *board, int *next) {
	int i, pos = -1;
	for (i = 0; i < 9; i++)
		if (board[i] == 0) pos = i;
	if (pos == -1) {
		printf("Invalid board state \n"); exit(1);
	} if (pos == 0 || pos == 1 || pos == 2) {
		/* Cannot be swapped with upper */
		return -1;
	} else {
		memcpy(next, board, sizeof(int) * 9);
		next[pos] = board[pos - 3]; next[pos - 3] = board[pos];
	}
	return 0;
}
int swap_with_lower(int *board, int *next) {
	int i, pos = -1;
	for (i = 0; i < 9; i++)
		if (board[i] == 0) pos = i;
	if (pos == -1) {
		printf("Invalid board state \n"); exit(1);
	} if (pos == 6 || pos == 7 || pos == 8) {
		/* Cannot be swapped with lower */
		return -1;
	} else {
		memcpy(next, board, sizeof(int) * 9);
		next[pos] = board[pos + 3]; next[pos + 3] = board[pos];
	}
	return 0;
}
int swap_with_right(int *board, int *next) {
	int i, pos = -1;
	for (i = 0; i < 9; i++)
		if (board[i] == 0) pos = i;
	if (pos == -1) {
		printf("Invalid board state \n"); exit(1);
	} if (pos == 2 || pos == 5 || pos == 8) {
		/* Cannot be swapped with right */
		return -1;
	} else {
		memcpy(next, board, sizeof(int) * 9);
		next[pos] = board[pos + 1]; next[pos + 1] = board[pos];
	}
	return 0;
}
int swap_with_left(int *board, int *next) {
	int i, pos = -1;
	for (i = 0; i < 9; i++)
		if (board[i] == 0) pos = i;
	if (pos == -1) {
		printf("Invalid board state \n"); exit(1);
	} if (pos == 0 || pos == 3 || pos == 6) {
		/* Cannot be swapped with right */
		return -1;
	} else {
		memcpy(next, board, sizeof(int) * 9);
		next[pos] = board[pos - 1]; next[pos - 1] = board[pos];
	}
	return 0;
}
/* Converter functions */
/* board[9]={1,2,3,8,0,4,7,6,5} --> val=123804765 */
long board2val(int *board) {
	int i, digit = 1;
	long val = 0;
	for (i = 0; i < 9; i++) {
		val += board[8 - i] * digit;
		digit *= 10;
	}
	return val;
}
/* val=123804765 --> board[9]={1,2,3,8,0,4,7,6,5} */
void val2board(long val, int *board) {
	int i, digit = 1;
	for (i = 0; i < 9; i++) {
		board[8 - i] = (val / digit) % 10;
		digit *= 10;
	}
}
/*** Queue functions ***/
#define QUEUE_NEXT(i) (((i) + 1) % QUEUE_SIZE)
long queue[QUEUE_SIZE];
int queue_head, queue_tail;
void Queue_Init() {
	queue_head = queue_tail = 0;
}
int Queue_Size() {
	return (queue_tail >= queue_head) ? (queue_tail - queue_head) :
			(QUEUE_SIZE - (queue_head - queue_tail));
}
void Queue_Push(long val) {
	if (QUEUE_NEXT(queue_tail) != queue_head) {
		queue[queue_tail] = val;
		queue_tail = QUEUE_NEXT(queue_tail);
	} else {
		printf("Queue overflow \n"); exit(1);
	}
}
long Queue_Pop() {
	long val;
	if (Queue_Size() <= 0) {
		printf("Queue underflow \n"); exit(1);
	}
	val = queue[queue_head];
	queue_head = QUEUE_NEXT(queue_head);
	return val;
}
/*** Hash functions ***/
#define HASH(i) ((i) % HASH_SIZE)
struct hashentry {
	long val;
	struct hashentry *next;
};
struct hashentry *hashtable[HASH_SIZE];
void Hash_Init() {
	int i;
	for (i = 0; i < HASH_SIZE; i++)
		hashtable[i] = NULL;
}
void Hash_Insert(long val) {
	struct hashentry *p;
	int h = HASH(val);
	if ((p = malloc(sizeof(struct hashentry))) == NULL) {
		printf("Memory allocation error\n"); exit(1);
	}
	/* Copy val to p */
	p->val = val;
	/* Insert p to the list */
	p->next = hashtable[h]; hashtable[h] = p;
}
int Hash_Search(long val) {
	struct hashentry *p;
	int h = HASH(val);
	/* If the node exists on the list, then return 1 */
	for (p = hashtable[h]; p != NULL; p = p->next)
		if (p->val == val) return 1;
	/* Otherwise return 0 */
	return 0;
}
void Hash_Clear() {
	int i;
	struct hashentry *p;
	for (i = 0; i < HASH_SIZE; i++) {
		while (hashtable[i] != NULL) {
			p = hashtable[i];
			hashtable[i] = p->next;
			free(p);
		}
	}
}