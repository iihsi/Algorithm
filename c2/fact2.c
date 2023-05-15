#include <stdio.h>
int fact(int n) {
	int val;
	printf("fact(%d) is called\n", n);
	if (n < 1) val = 1;
	else       val = n * fact(n - 1);
	printf("fact(%d) = %d\n", n, val);
	return val;
}
int main() {
	int n;
	printf("Input positive integer: ");
	scanf("%d", &n);
	fact(n);
	return 0;
}