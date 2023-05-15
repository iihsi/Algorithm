#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEPTH 10 
#define EVAL_FAIR 0
#define EVAL_WEAK 1
#define EVAL_BAD 2
int counter_compare = 0;
int eval_func = 0;
int fair_evaluator(int *board, int *goal);
int weak_evaluator(int *board, int *goal);
int bad_evaluator(int *board, int *goal);
int swap_with_upper(int *board, int *next);
int swap_with_lower(int *board, int *next);
int swap_with_right(int *board, int *next);
int swap_with_left(int *board, int *next);
long board2val(int *board);
void val2board(long val, int *board);
/*** Heap/Hash functions ***/
#define HASH_SIZE 1024
#define HEAP_SIZE 1024
void PQ_Init();
int PQ_Size();
void PQ_Push(long val, int depth, int score);
void PQ_RemoveMin(long *val, int *depth);
void PQ_Disp();
void Hash_Init();
void Hash_Insert(long val);
int Hash_Search(long val);
void Hash_Clear();
/***************************/
int my_compare(int *board1, int *board2) {
	counter_compare++;
	return memcmp(board1, board2, sizeof(int) * 9);
}
void print_board(int *board, int depth, int score) {
	printf("Depth: %d (score: %d)\n", depth, score);
	printf("%2d%2d%2d\n", board[0], board[1], board[2]);
	printf("%2d%2d%2d\n", board[3], board[4], board[5]);
	printf("%2d%2d%2d\n", board[6], board[7], board[8]);
}
/* Calculate score of board state */
int score(int *board, int *goal, int depth) {
	switch (eval_func) {
	case EVAL_FAIR: return fair_evaluator(board, goal) + depth;
	case EVAL_WEAK: return weak_evaluator(board, goal) + depth;
	default:        return bad_evaluator(board, goal) + depth;
	}
}
/* Push board state to Priorty Queue */
void astar_push(int *board, int *goal, int depth) {
	PQ_Push(board2val(board), depth, score(board, goal, depth));
}
/* Pop board state from Priorty Queue */
void astar_pop(int *board, int *depth) {
	long val;
	PQ_RemoveMin(&val, depth);
	val2board(val, board);
}
int astar_empty() {
	return PQ_Size() == 0;
}
/* Set board state as "visited" */
void set_visited(int *board) {
	Hash_Insert(board2val(board));
}
/* Is board state "visited"? */
int is_visited(int *board) {
	return Hash_Search(board2val(board));
}
void Astar(int *init, int *goal) {
	int depth = 0;
	astar_push(init, goal, depth);
	print_board(init, depth, score(init, goal, depth));
	if (!my_compare(init, goal)) {
		printf("Found!\n"); return;
	}
	while (!astar_empty()) {
		int curr[9], next[9];
		astar_pop(curr, &depth);
		set_visited(curr);
		print_board(curr, depth, score(curr, goal, depth));
		if (!my_compare(curr, goal)) {
			printf("Found!\n"); return;
		}
		depth++;
		/* Swap with upper */
		if (swap_with_upper(curr, next) != -1)
			if (!is_visited(next) && depth < MAX_DEPTH)
				astar_push(next, goal, depth);
		/* Swap with lower */
		if (swap_with_lower(curr, next) != -1)
			if (!is_visited(next) && depth < MAX_DEPTH)
				astar_push(next, goal, depth);
		/* Swap with right */
		if (swap_with_right(curr, next) != -1)
			if (!is_visited(next) && depth < MAX_DEPTH)
				astar_push(next, goal, depth);
		/* Swap with left */
		if (swap_with_left(curr, next) != -1)
			if (!is_visited(next) && depth < MAX_DEPTH)
				astar_push(next, goal, depth);
	}
	printf("Not Found...\n");
}
int main(int argc, char *argv[]) {
	FILE *fp;
	char command;
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

	printf("[f]	Fair Evaluator\n");
	printf("[w]	Weak Evaluator\n");
	printf("[b]	Bad Evaluator\n");
	printf("[e]	Exit\n");
	for (;;) {
		scanf(" %c", &command);
		switch (command) {
		case 'f':	eval_func = EVAL_FAIR; break;
		case 'w':	eval_func = EVAL_WEAK; break;
		case 'b':	eval_func = EVAL_BAD; break;
		case 'e':	Hash_Clear(); return 0;
		}
		counter_compare = 0;
		PQ_Init();
		Hash_Init();
		Astar(init, goal);
		printf("Compare %d\n", counter_compare);
	}
	return 0;
}
/* Evaluation functions */
int fair_evaluator(int *board, int *goal) {
	int i, j, score = 0;
	for (i = 1; i < 9; i++) {
		int x1, y1, x2, y2;
		for (j = 0; j < 9; j++) {
			if (board[j] == i) {
				x1 = j % 3;
				y1 = j / 3;
			}
			if (goal[j] == i) {
				x2 = j % 3;
				y2 = j / 3;
			}
		}
		score += abs(x1 - x2) + abs(y1 - y2);
	}
	return score;
}
int weak_evaluator(int *board, int *goal) {
	int i, score = 0;
	for (i = 0; i < 9; i++)
		if (board[i] != 0 && board[i] != goal[i]) score++;
	return score;
}
int bad_evaluator(int *board, int *goal) {
	int diff = 0;
	diff += board[8] - board[0];
	diff += board[7] - board[1];
	diff += board[6] - board[2];
	diff += board[3] - board[5];
	return 16 - diff;
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
/*** Heap functions ***/
struct heapentry {
	long val;
	int depth;
	int score;
};
struct heapentry Heap[HEAP_SIZE];
int Heap_Size;
void downheap(int n){
	for (;;) {
		struct heapentry tmp;
		int c = 2 * n + 1;
		if (c >= Heap_Size) break;
		if (c + 1 < Heap_Size && Heap[c].score > Heap[c + 1].score) c++;
		if (Heap[n].score <= Heap[c].score) break;
		memcpy(&tmp, &Heap[n], sizeof(struct heapentry));
		memcpy(&Heap[n], &Heap[c], sizeof(struct heapentry));
		memcpy(&Heap[c], &tmp, sizeof(struct heapentry));
		n = c; 
	}
}
void upheap(int n){
	for (;;) {
		struct heapentry tmp;
		int p = (n - 1) / 2;
		if (p < 0 || Heap[p].score <= Heap[n].score) break;
		memcpy(&tmp, &Heap[n], sizeof(struct heapentry));
		memcpy(&Heap[n], &Heap[p], sizeof(struct heapentry));
		memcpy(&Heap[p], &tmp, sizeof(struct heapentry));
		n = p;
	}
}
void PQ_Init() {
	Heap_Size = 0;
}
int PQ_Size() {
	return Heap_Size;
}
void PQ_Push(long val, int depth, int score) {
	if (Heap_Size >= HEAP_SIZE) {
		printf("Priority queue overflow \n"); exit(1);
	}
	Heap[Heap_Size].val = val;
	Heap[Heap_Size].depth = depth;
	Heap[Heap_Size].score = score;
	upheap(Heap_Size);
	Heap_Size++;
}
void PQ_RemoveMin(long *val, int *depth) {
	if (Heap_Size <= 0) {
		printf("Priority queue underflow \n"); exit(1);
	}
	*val = Heap[0].val;
	*depth = Heap[0].depth;
	memcpy(&Heap[0], &Heap[--Heap_Size], sizeof(struct heapentry));
	downheap(0);
}
void PQ_Disp() {
	int i;
	for (i = 0; i < Heap_Size; i++)
		printf("(%ld %d) ", Heap[i].val, Heap[i].score);
	printf("\n");
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