#include <stdio.h>

char A[] = "ABCBDAB";
char B[] = "BDCABA";

#define max(x, y) (x)>(y)? (x) : (y)
int step = 0;

int C(int i, int j)
{
	step++;
	if (i == 0) {
		return 0;
	}
	if (j == 0) {
		return 0;
	}
	if (*(A+i-1) == *(B+j-1)) {
		return C(i-1, j-1) + 1;
	} else {
		return max(C(i - 1, j), C(i, j - 1));
	}
}

int main()
{
	printf ("LCS is %d.\n", C(7, 6));
	printf ("Step is %d.\n", step);
	return 0;
}

