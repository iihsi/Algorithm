#include <stdio.h>
int fact(int n) {
	if (n < 1) return 1;
	return n * fact(n - 1);
}
int main() {
	int n;
	printf("Input positive integer: ");
	scanf("%d", &n);
	printf("fact(%d) = %d\n", n, fact(n));
	return 0;
}